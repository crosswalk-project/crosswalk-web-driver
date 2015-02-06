// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_ANDROID_DEVICE_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_ANDROID_DEVICE_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "xwalk/test/xwalkdriver/xwalk/device.h"

class DeviceBridge;
class Status;
class DeviceManager;

class AndroidDevice : public Device{
 public:
  ~AndroidDevice() override;

  Status SetUp(const std::string& app_id,
               const std::string& args,
               int local_port,
               int remote_port) override;

  Status TearDown() override;

 private:
  friend class DeviceManager;

  AndroidDevice(const std::string& device_serial,
                DeviceBridge* device_bridge,
                base::Callback<void()> release_callback);

  Status ForwardDevtoolsPort(const std::string& package,
                             int local_port);

  const std::string serial_;
  std::string active_app_;
  DeviceBridge* device_bridge_;
  base::Callback<void()> release_callback_;

  DISALLOW_COPY_AND_ASSIGN(AndroidDevice);
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_ANDROID_DEVICE_H_
