// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_MANAGER_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_MANAGER_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"

class DeviceBridge;
class Status;
class Device;

class DeviceManager {
 public:
  explicit DeviceManager(DeviceBridge* device_bridge);
  ~DeviceManager();

  // Returns a device which will not be reassigned during its lifetime.
  Status AcquireDevice(scoped_ptr<Device>* device);

  // Returns a device with the same guarantees as AcquireDevice, but fails
  // if the device with the given serial number is not avaliable.
  Status AcquireSpecificDevice(const std::string& device_serial,
                               scoped_ptr<Device>* device);

 private:
  void ReleaseDevice(const std::string& device_serial);

  Device* LockDevice(const std::string& device_serial);
  bool IsDeviceLocked(const std::string& device_serial);

  base::Lock devices_lock_;
  std::list<std::string> active_devices_;
  DeviceBridge* device_bridge_;

  DISALLOW_COPY_AND_ASSIGN(DeviceManager);
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_MANAGER_H_
