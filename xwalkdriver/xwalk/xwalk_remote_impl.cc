// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/test/xwalkdriver/xwalk/xwalk_remote_impl.h"
#include "xwalk/test/xwalkdriver/xwalk/devtools_client.h"
#include "xwalk/test/xwalkdriver/xwalk/devtools_http_client.h"
#include "xwalk/test/xwalkdriver/xwalk/status.h"
#include "xwalk/test/xwalkdriver/net/port_server.h"

XwalkRemoteImpl::XwalkRemoteImpl(
    scoped_ptr<DevToolsHttpClient> http_client,
    scoped_ptr<DevToolsClient> websocket_client,
    ScopedVector<DevToolsEventListener>& devtools_event_listeners)
    : XwalkImpl(http_client.Pass(),
                 websocket_client.Pass(),
                 devtools_event_listeners,
                 scoped_ptr<PortReservation>()) {}

XwalkRemoteImpl::~XwalkRemoteImpl() {}

Status XwalkRemoteImpl::GetAsDesktop(XwalkDesktopImpl** desktop) {
  return Status(kUnknownError,
                "operation is unsupported with remote debugging");
}

std::string XwalkRemoteImpl::GetOperatingSystemName() {
 return std::string();
}

Status XwalkRemoteImpl::QuitImpl() {
  return Status(kOk);
}

