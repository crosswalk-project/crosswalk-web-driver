// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/device_manager.h"

#include <algorithm>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_util.h"
#include "xwalk/test/xwalkdriver/xwalk/adb_impl.h"
#include "xwalk/test/xwalkdriver/xwalk/android_device.h"
#include "xwalk/test/xwalkdriver/xwalk/device.h"
#include "xwalk/test/xwalkdriver/xwalk/device_bridge.h"
#include "xwalk/test/xwalkdriver/xwalk/sdb_impl.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"
#include "xwalk/test/xwalkdriver/xwalk/tizen_device.h"

DeviceManager::DeviceManager(
    const scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    int port,
    int device_type) {
  if (device_type == internal::kAndroid)
    device_bridge_.reset(new AdbImpl(io_task_runner, port));
  else if (device_type == internal::kTizen)
    device_bridge_.reset(new SdbImpl(io_task_runner, port));
  else
    ;
  CHECK(device_bridge_);
}

DeviceManager::~DeviceManager() {}

Status DeviceManager::AcquireDevice(scoped_ptr<Device>* device) {
  std::vector<std::string> devices;
  Status status = device_bridge_.get()->GetDevices(&devices);
  if (status.IsError())
    return status;

  if (devices.empty())
    return Status(kUnknownError, "There are no devices online");

  base::AutoLock lock(devices_lock_);
  status = Status(kUnknownError, "All devices are in use (" +
                  base::IntToString(devices.size()) + " online)");
  std::vector<std::string>::iterator iter;
  for (iter = devices.begin(); iter != devices.end(); iter++) {
    if (IsDeviceLocked(*iter)) {
      active_devices_.remove(*iter);
      device->reset(LockDevice(*iter));
      status = Status(kOk);
      break;
    } else {
      device->reset(LockDevice(*iter));
      status = Status(kOk);
      break;
    }
  }
  return status;
}

Status DeviceManager::AcquireSpecificDevice(
    const std::string& device_serial, scoped_ptr<Device>* device) {
  std::vector<std::string> devices;
  Status status = device_bridge_.get()->GetDevices(&devices);
  if (status.IsError())
    return status;

  for (std::vector<std::string>::const_iterator iter = devices.begin();
       iter != devices.end(); ++iter) {
    if ((*iter).find(device_serial) != std::string::npos) {
      base::AutoLock lock(devices_lock_);
      if (IsDeviceLocked(*iter)) {
        // since we add authorization for connecting xwalkdriver by 
        // whitelisted-ips now, we make it possible to relaunch the running
        // app by authorized developers who should take care of it and make
        // their own decisions whether the running app can be interruptable.
        active_devices_.remove(*iter);
      }
      device->reset(LockDevice(*iter));
      return Status(kOk);
    }
  }    
  
  return Status(kUnknownError, "Device " + device_serial + " is not online");
}

void DeviceManager::ReleaseDevice(const std::string& device_serial) {
  base::AutoLock lock(devices_lock_);
  active_devices_.remove(device_serial);
}

Device* DeviceManager::LockDevice(const std::string& device_serial) {
  active_devices_.push_back(device_serial);
  std::string os_name = base::StringToLowerASCII(
                            device_bridge_.get()->GetOperatingSystemName());

  if (os_name.find("android") != std::string::npos)
    return new AndroidDevice(device_serial, device_bridge_.get(),
        base::Bind(&DeviceManager::ReleaseDevice, base::Unretained(this),
                   device_serial));
  else
    return new TizenDevice(device_serial, device_bridge_.get(),
        base::Bind(&DeviceManager::ReleaseDevice, base::Unretained(this),
                   device_serial));
}

bool DeviceManager::IsDeviceLocked(const std::string& device_serial) {
  return std::find(active_devices_.begin(), active_devices_.end(),
                   device_serial) != active_devices_.end();
}

