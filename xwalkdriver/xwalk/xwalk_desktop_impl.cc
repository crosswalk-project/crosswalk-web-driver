// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/xwalk_desktop_impl.h"

#include "base/bind.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/posix/eintr_wrapper.h"
#include "base/process/kill.h"
#include "base/sys_info.h"
#include "base/threading/platform_thread.h"
#include "base/time/time.h"
#include "xwalk/test/xwalkdriver/xwalk/automation_extension.h"
#include "xwalk/test/xwalkdriver/xwalk/devtools_client.h"
#include "xwalk/test/xwalkdriver/xwalk/devtools_http_client.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"
#include "xwalk/test/xwalkdriver/xwalk/web_view_impl.h"
#include "xwalk/test/xwalkdriver/net/port_server.h"

#if defined(OS_POSIX)
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace {

bool KillProcess(const base::Process& process, bool kill_gracefully) {
#if defined(OS_POSIX)
  if (!kill_gracefully) {
    kill(process.Pid(), SIGKILL);
    base::TimeTicks deadline =
        base::TimeTicks::Now() + base::TimeDelta::FromSeconds(30);
    while (base::TimeTicks::Now() < deadline) {
      pid_t pid = HANDLE_EINTR(waitpid(process.Pid(), NULL, WNOHANG));
      if (pid == process.Pid())
        return true;
      if (pid == -1) {
        if (errno == ECHILD) {
          // The wait may fail with ECHILD if another process also waited for
          // the same pid, causing the process state to get cleaned up.
          return true;
        }
        LOG(WARNING) << "Error waiting for process " << process.Pid();
      }
      base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(50));
    }
    return false;
  }
#endif

  if (!process.Terminate(0, true)) {
    int exit_code;
    return base::GetTerminationStatus(process.Handle(), &exit_code) !=
        base::TERMINATION_STATUS_STILL_RUNNING;
  }
  return true;
}

void GetWebViewIdForAppWindow(const std::string& url_prefix,
                              std::string* web_view_id,
                              const WebViewInfo& view) {
  if (view.type == WebViewInfo::kApp && view.url.find(url_prefix) == 0)
    *web_view_id = view.id;
}

}  // namespace

XwalkDesktopImpl::XwalkDesktopImpl(
    scoped_ptr<DevToolsHttpClient> http_client,
    scoped_ptr<DevToolsClient> websocket_client,
    ScopedVector<DevToolsEventListener>& devtools_event_listeners,
    scoped_ptr<PortReservation> port_reservation,
    base::Process process,
    const base::CommandLine& command,
    base::ScopedTempDir* user_data_dir,
    base::ScopedTempDir* extension_dir)
    : XwalkImpl(http_client.Pass(),
                 websocket_client.Pass(),
                 devtools_event_listeners,
                 port_reservation.Pass()),
      process_(process.Pass()),
      command_(command) {
  if (user_data_dir->IsValid())
    CHECK(user_data_dir_.Set(user_data_dir->Take()));
  if (extension_dir->IsValid())
    CHECK(extension_dir_.Set(extension_dir->Take()));
}

XwalkDesktopImpl::~XwalkDesktopImpl() {
  if (!quit_) {
    base::FilePath user_data_dir = user_data_dir_.Take();
    base::FilePath extension_dir = extension_dir_.Take();
    LOG(WARNING) << "xwalk quit unexpectedly, leaving behind temporary "
        "directories for debugging:";
    if (user_data_dir_.IsValid())
      LOG(WARNING) << "xwalk user data directory: " << user_data_dir.value();
    if (extension_dir_.IsValid())
      LOG(WARNING) << "xwalkdriver automation extension directory: "
                   << extension_dir.value();
  }
}

Status XwalkDesktopImpl::WaitForPageToLoad(const std::string& url,
                                            const base::TimeDelta& timeout,
                                            scoped_ptr<WebView>* web_view) {
  base::TimeTicks deadline = base::TimeTicks::Now() + timeout;
  std::string id;
  while (base::TimeTicks::Now() < deadline) {
    WebViewsInfo views_info;
    Status status = devtools_http_client_->GetWebViewsInfo(&views_info);
    if (status.IsError())
      return status;

    for (size_t i = 0; i < views_info.GetSize(); ++i) {
      if (views_info.Get(i).url.find(url) == 0) {
        id = views_info.Get(i).id;
        break;
      }
    }
    if (!id.empty())
      break;
    base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(100));
  }
  if (id.empty())
    return Status(kUnknownError, "page could not be found: " + url);

  scoped_ptr<WebView> web_view_tmp(
      new WebViewImpl(id,
                      devtools_http_client_->browser_info(),
                      devtools_http_client_->CreateClient(id),
                      devtools_http_client_->device_metrics()));
  Status status = web_view_tmp->ConnectIfNecessary();
  if (status.IsError())
    return status;

  status = web_view_tmp->WaitForPendingNavigations(
      std::string(), deadline - base::TimeTicks::Now(), false);
  if (status.IsOk())
    *web_view = web_view_tmp.Pass();
  return status;
}

Status XwalkDesktopImpl::GetAutomationExtension(
    AutomationExtension** extension) {
  if (!automation_extension_) {
    scoped_ptr<WebView> web_view;
    Status status = WaitForPageToLoad(
        "xwalk-extension://aapnijgdinlhnhlmodcfapnahmbfebeb/"
        "_generated_background_page.html",
        base::TimeDelta::FromSeconds(10),
        &web_view);
    if (status.IsError())
      return Status(kUnknownError, "cannot get automation extension", status);

    automation_extension_.reset(new AutomationExtension(web_view.Pass()));
  }
  *extension = automation_extension_.get();
  return Status(kOk);
}

Status XwalkDesktopImpl::GetAsDesktop(XwalkDesktopImpl** desktop) {
  *desktop = this;
  return Status(kOk);
}

std::string XwalkDesktopImpl::GetOperatingSystemName() {
  return base::SysInfo::OperatingSystemName();
}

bool XwalkDesktopImpl::IsMobileEmulationEnabled() const {
  return devtools_http_client_->device_metrics() != NULL;
}

bool XwalkDesktopImpl::HasTouchScreen() const {
  return IsMobileEmulationEnabled();
}

Status XwalkDesktopImpl::QuitImpl() {
  // If the Xwalk session uses a custom user data directory, try sending a
  // SIGTERM signal before SIGKILL, so that Xwalk has a chance to write
  // everything back out to the user data directory and exit cleanly.If
  // we're using a temporary user data directory, we're going to delete
  // the temporary directory anyway, so just send SIGKILL immediately.
  if (!KillProcess(process_, !user_data_dir_.IsValid()))
    return Status(kUnknownError, "cannot kill Xwalk");
  return Status(kOk);
}

const base::CommandLine& XwalkDesktopImpl::command() const {
  return command_;
}

Status XwalkDesktopImpl::WaitForNewAppWindow(const base::TimeDelta& timeout,
                                              const std::string& app_id,
                                              std::string* web_view_id) {
  base::TimeTicks deadline = base::TimeTicks::Now() + timeout;
  std::string url_prefix = "xwalk-extension://" + app_id;
  std::string web_view_id_tmp;
  std::list<std::string> web_view_ids;
  WebViewCallback callback =
      base::Bind(&GetWebViewIdForAppWindow, url_prefix, &web_view_id_tmp);
  while (base::TimeTicks::Now() < deadline) {
    Status status = UpdateWebViewIds(&web_view_ids, callback);
    if (status.IsError())
      return status;
    if (!web_view_id_tmp.empty()) {
      *web_view_id = web_view_id_tmp;
      return Status(kOk);
    }
    base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(50));
  }
  return Status(kUnknownError, "timed out waiting for app window to appear");
}
