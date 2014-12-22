// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"

class Status;
class DeviceManager;

class Device {
 public:
  Device() {}
  virtual ~Device() {}

  virtual Status SetUp(const std::string& app_id,
                       const std::string& args,
                       int port,
                       int remote_port) = 0;
  virtual Status TearDown() = 0;

 private:
  friend class DeviceManager;
  DISALLOW_COPY_AND_ASSIGN(Device);
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_DEVICE_H_
