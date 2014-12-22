// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_SDB_IMPL_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_SDB_IMPL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "xwalk/test/xwalkdriver/xwalk/device_bridge.h"

namespace base {
class SingleThreadTaskRunner;
}

class Status;

class SdbImpl : public DeviceBridge {
 public:
  explicit SdbImpl(
      const scoped_refptr<base::SingleThreadTaskRunner>& io_message_loop_proxy,
      int port);
  virtual ~SdbImpl();

  bool IsTizenAppRunning(const std::string& device_serial,
                         const std::string& app_id);

  // Overridden from DeviceBridge:
  virtual Status GetDevices(std::vector<std::string>* devices) OVERRIDE;
  virtual Status ForwardPort(const std::string& device_serial,
                             int local_port,
                             const std::string& remote_port) OVERRIDE;
  virtual Status SetCommandLineFile(const std::string& device_serial,
                                    const std::string& command_line_file,
                                    const std::string& exec_name,
                                    const std::string& args) OVERRIDE;
  virtual Status CheckAppInstalled(const std::string& device_serial,
                                   const std::string& app_id) OVERRIDE;
  virtual Status ClearAppData(const std::string& device_serial,
                              const std::string& app_id) OVERRIDE;
  virtual Status SetDebugApp(const std::string& device_serial,
                             const std::string& app_id) OVERRIDE;
  virtual Status Launch(const std::string& device_serial,
                        const std::string& app_id) OVERRIDE;
  virtual Status ForceStop(const std::string& device_serial,
                           const std::string& app_id) OVERRIDE;
  virtual Status GetPidByName(const std::string& device_serial,
                              const std::string& process_name,
                              int* pid) OVERRIDE;
  virtual std::string GetOperatingSystemName() OVERRIDE;
 private:
  Status ExecuteCommand(const std::string& command,
                        std::string* response);
  Status ExecuteHostCommand(const std::string& device_serial,
                            const std::string& host_command,
                            std::string* response);
  Status ExecuteHostShellCommand(const std::string& device_serial,
                                 const std::string& shell_command,
                                 std::string* response);

  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner_;

  int port_;
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_SDB_IMPL_H_
