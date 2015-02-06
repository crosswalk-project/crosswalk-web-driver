// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_STUB_XWALK_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_STUB_XWALK_H_

#include <list>
#include <string>

#include "base/compiler_specific.h"
#include "xwalk/test/xwalkdriver/xwalk/xwalk.h"

class Status;
class WebView;

class StubXwalk : public Xwalk {
 public:
  StubXwalk();
  ~StubXwalk() override;

  // Overridden from Xwalk:
  XwalkDesktopImpl* GetAsDesktop() override;
  std::string GetVersion() override;
  int GetBuildNo() override;
  bool HasCrashedWebView() override;
  Status GetWebViewIds(std::list<std::string>* web_view_ids) override;
  Status GetWebViewById(const std::string& id,
                        WebView** web_view) override;
  Status CloseWebView(const std::string& id) override;
  Status ActivateWebView(const std::string& id) override;
  std::string GetOperatingSystemName() override;
  Status Quit() override;
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_STUB_XWALK_H_
