// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/android_device.h"

#include <algorithm>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "xwalk/test/xwalkdriver/xwalk/device_bridge.h"
#include "xwalk/test/xwalkdriver/xwalk/device_manager.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"

AndroidDevice::AndroidDevice(
    const std::string& device_serial, DeviceBridge* device_bridge,
    base::Callback<void()> release_callback)
    : serial_(device_serial),
      device_bridge_(device_bridge),
      release_callback_(release_callback) {}

AndroidDevice::~AndroidDevice() {
  release_callback_.Run();
}

Status AndroidDevice::SetUp(const std::string& app_id,
                            const std::string& args,
                            int local_port,
                            int remote_port) {
  std::size_t pos = app_id.find("/");
  std::string package = app_id.substr(0, pos);
  std::string activity = app_id.substr(pos+1);

  if (!active_app_.empty())
    return Status(kUnknownError,
        active_app_ + " was launched and has not been quit");

  Status status = device_bridge_->CheckAppInstalled(serial_, package);
  if (status.IsError())
    return status;

  std::string command_line_file;
  std::string exec_name;

  status = device_bridge_->ClearAppData(serial_, package);
  if (status.IsError())
    return status;

  if (!command_line_file.empty()) {
    status = device_bridge_->SetCommandLineFile(
        serial_, command_line_file, exec_name, args);
    if (status.IsError())
      return status;
  }

  status = device_bridge_->Launch(serial_, app_id);

  if (status.IsError())
    return status;

  active_app_ = package;
  return ForwardDevtoolsPort(package, local_port);
}

Status AndroidDevice::ForwardDevtoolsPort(const std::string& package,
                                   int local_port) {
  int pid;
  Status status = device_bridge_->GetPidByName(serial_, package, &pid);

  if (status.IsError()) 
    return status;

  std::string remote_abstract;
  remote_abstract = base::StringPrintf("%s_devtools_remote", package.c_str());
  VLOG(0) << "Device socket is: " + remote_abstract;
  
  return device_bridge_->ForwardPort(serial_, local_port, remote_abstract);
}

Status AndroidDevice::TearDown() {
  if (!active_app_.empty()) {
    std::string response;
    Status status = device_bridge_->ForceStop(serial_, active_app_);
    if (status.IsError())
      return status;
    active_app_ = "";
  }
  return Status(kOk);
}

