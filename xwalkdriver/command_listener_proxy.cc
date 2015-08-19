// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/devtools_client.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"
#include "xwalk/test/xwalkdriver/command_listener_proxy.h"
#include "xwalk/test/xwalkdriver/logging.h"

CommandListenerProxy::CommandListenerProxy(
    CommandListener* command_listener) : command_listener_(command_listener) {
  CHECK(command_listener_);
}

CommandListenerProxy::~CommandListenerProxy() { }

Status CommandListenerProxy::BeforeCommand(const std::string& command_name) {
  return command_listener_->BeforeCommand(command_name);
}
