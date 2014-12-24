// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_WEB_VIEW_IMPL_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_WEB_VIEW_IMPL_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "xwalk/test/xwalkdriver/xwalk/web_view.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

class DebuggerTracker;
class DevToolsClient;
class DomTracker;
class FrameTracker;
class GeolocationOverrideManager;
class HeapSnapshotTaker;
struct KeyEvent;
struct MouseEvent;
class NavigationTracker;
class Status;

class WebViewImpl : public WebView {
 public:
  WebViewImpl(const std::string& id,
              int build_no,
              scoped_ptr<DevToolsClient> client);
  virtual ~WebViewImpl();

  // Overridden from WebView:
  virtual std::string GetId() override;
  virtual bool WasCrashed() override;
  virtual Status ConnectIfNecessary() override;
  virtual Status HandleReceivedEvents() override;
  virtual Status Load(const std::string& url) override;
  virtual Status Reload() override;
  virtual Status EvaluateScript(const std::string& frame,
                                const std::string& expression,
                                scoped_ptr<base::Value>* result) override;
  virtual Status CallFunction(const std::string& frame,
                              const std::string& function,
                              const base::ListValue& args,
                              scoped_ptr<base::Value>* result) override;
  virtual Status CallAsyncFunction(const std::string& frame,
                                   const std::string& function,
                                   const base::ListValue& args,
                                   const base::TimeDelta& timeout,
                                   scoped_ptr<base::Value>* result) override;
  virtual Status CallUserAsyncFunction(
      const std::string& frame,
      const std::string& function,
      const base::ListValue& args,
      const base::TimeDelta& timeout,
      scoped_ptr<base::Value>* result) override;
  virtual Status GetFrameByFunction(const std::string& frame,
                                    const std::string& function,
                                    const base::ListValue& args,
                                    std::string* out_frame) override;
  virtual Status DispatchMouseEvents(
      const std::list<MouseEvent>& events, const std::string& frame) override;
  virtual Status DispatchTouchEvents(
      const std::list<TouchEvent>& events) override;
  virtual Status DispatchKeyEvents(const std::list<KeyEvent>& events) override;
  virtual Status GetCookies(scoped_ptr<base::ListValue>* cookies) override;
  virtual Status DeleteCookie(const std::string& name,
                              const std::string& url) override;
  virtual Status WaitForPendingNavigations(const std::string& frame_id,
                                           const base::TimeDelta& timeout,
                                           bool stop_load_on_timeout) override;
  virtual Status IsPendingNavigation(
      const std::string& frame_id, bool* is_pending) override;
  virtual JavaScriptDialogManager* GetJavaScriptDialogManager() override;
  virtual Status OverrideGeolocation(const Geoposition& geoposition) override;
  virtual Status CaptureScreenshot(std::string* screenshot) override;
  virtual Status SetFileInputFiles(
      const std::string& frame,
      const base::DictionaryValue& element,
      const std::vector<base::FilePath>& files) override;  // NOLINT
  virtual Status TakeHeapSnapshot(scoped_ptr<base::Value>* snapshot) override;

 private:
  Status CallAsyncFunctionInternal(const std::string& frame,
                                   const std::string& function,
                                   const base::ListValue& args,
                                   bool is_user_supplied,
                                   const base::TimeDelta& timeout,
                                   scoped_ptr<base::Value>* result);
  Status IsNotPendingNavigation(const std::string& frame_id,
                                bool* is_not_pending);
  std::string id_;
  int build_no_;
  scoped_ptr<DomTracker> dom_tracker_;
  scoped_ptr<FrameTracker> frame_tracker_;
  scoped_ptr<NavigationTracker> navigation_tracker_;
  scoped_ptr<JavaScriptDialogManager> dialog_manager_;
  scoped_ptr<GeolocationOverrideManager> geolocation_override_manager_;
  scoped_ptr<HeapSnapshotTaker> heap_snapshot_taker_;
  scoped_ptr<DebuggerTracker> debugger_;
  scoped_ptr<DevToolsClient> client_;
};

namespace internal {

enum EvaluateScriptReturnType {
  ReturnByValue,
  ReturnByObject
};
Status EvaluateScript(DevToolsClient* client,
                      int context_id,
                      const std::string& expression,
                      EvaluateScriptReturnType return_type,
                      scoped_ptr<base::DictionaryValue>* result);
Status EvaluateScriptAndGetObject(DevToolsClient* client,
                                  int context_id,
                                  const std::string& expression,
                                  bool* got_object,
                                  std::string* object_id);
Status EvaluateScriptAndGetValue(DevToolsClient* client,
                                 int context_id,
                                 const std::string& expression,
                                 scoped_ptr<base::Value>* result);
Status ParseCallFunctionResult(const base::Value& temp_result,
                               scoped_ptr<base::Value>* result);
Status GetNodeIdFromFunction(DevToolsClient* client,
                             int context_id,
                             const std::string& function,
                             const base::ListValue& args,
                             bool* found_node,
                             int* node_id);

}  // namespace internal

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_WEB_VIEW_IMPL_H_
