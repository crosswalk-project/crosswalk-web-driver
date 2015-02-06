// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/tizen_device.h"

#include <algorithm>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "xwalk/test/xwalkdriver/xwalk/device.h"
#include "xwalk/test/xwalkdriver/xwalk/device_bridge.h"
#include "xwalk/test/xwalkdriver/xwalk/device_manager.h"
#include "xwalk/test/xwalkdriver/xwalk/sdb_impl.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"

TizenDevice::TizenDevice(
    const std::string& device_serial, DeviceBridge* device_bridge,
    base::Callback<void()> release_callback)
    : serial_(device_serial),
      device_bridge_(device_bridge),
      release_callback_(release_callback) {}

TizenDevice::~TizenDevice() {
  release_callback_.Run();
}

Status TizenDevice::SetUp(const std::string& app_id,
                     const std::string& args,
                     int local_port,
                     int remote_port) {
  Status status = Status(kOk);
  if (!active_app_.empty())
    return Status(kUnknownError,
        active_app_ + " was launched and has not been quit");

  SdbImpl* sdb = static_cast<SdbImpl*>(device_bridge_);
  status = sdb->DetectUserAccountName(serial_);
  if (status.IsError())
    return status;

  status = device_bridge_->CheckAppInstalled(serial_, app_id);
  if (status.IsError())
    return status;

  status = device_bridge_->Launch(serial_, app_id);
  if (status.IsError())
    return status;

  active_app_ = app_id;
  status = device_bridge_->ForwardPort(serial_, local_port, base::IntToString(remote_port));
  return status;
}

Status TizenDevice::TearDown() {
  if (!active_app_.empty()) {
    std::string response;
    Status status = device_bridge_->ForceStop(serial_, active_app_);
    if (status.IsError())
      return status;
    active_app_ = "";
  }
  return Status(kOk);
}

