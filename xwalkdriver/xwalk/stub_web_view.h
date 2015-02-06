// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_TEST_XWALKDRIVER_XWALK_STUB_WEB_VIEW_H_
#define XWALK_TEST_XWALKDRIVER_XWALK_STUB_WEB_VIEW_H_

#include <list>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "xwalk/test/xwalkdriver/xwalk/web_view.h"

class StubWebView : public WebView {
 public:
  explicit StubWebView(const std::string& id);
  ~StubWebView() override;

  // Overridden from WebView:
  std::string GetId() override;
  bool WasCrashed() override;
  Status ConnectIfNecessary() override;
  Status HandleReceivedEvents() override;
  Status Load(const std::string& url) override;
  Status Reload() override;
  Status EvaluateScript(const std::string& frame,
                        const std::string& function,
                        scoped_ptr<base::Value>* result) override;
  Status CallFunction(const std::string& frame,
                      const std::string& function,
                      const base::ListValue& args,
                      scoped_ptr<base::Value>* result) override;
  Status CallAsyncFunction(const std::string& frame,
                           const std::string& function,
                           const base::ListValue& args,
                           const base::TimeDelta& timeout,
                           scoped_ptr<base::Value>* result) override;
  Status CallUserAsyncFunction(const std::string& frame,
                               const std::string& function,
                               const base::ListValue& args,
                               const base::TimeDelta& timeout,
                               scoped_ptr<base::Value>* result) override;
  Status GetFrameByFunction(const std::string& frame,
                            const std::string& function,
                            const base::ListValue& args,
                            std::string* out_frame) override;
  Status DispatchMouseEvents(const std::list<MouseEvent>& events,
                             const std::string& frame) override;
  Status DispatchTouchEvents(const std::list<TouchEvent>& events) override;
  Status DispatchKeyEvents(const std::list<KeyEvent>& events) override;
  Status GetCookies(scoped_ptr<base::ListValue>* cookies) override;
  Status DeleteCookie(const std::string& name, const std::string& url) override;
  Status WaitForPendingNavigations(const std::string& frame_id,
                                   const base::TimeDelta& timeout,
                                   bool stop_load_on_timeout) override;
  Status IsPendingNavigation(const std::string& frame_id,
                             bool* is_pending) override;
  JavaScriptDialogManager* GetJavaScriptDialogManager() override;
  Status OverrideGeolocation(const Geoposition& geoposition) override;
  Status CaptureScreenshot(std::string* screenshot) override;
  Status SetFileInputFiles(const std::string& frame,
                           const base::DictionaryValue& element,
                           const std::vector<base::FilePath>& files) override;
  Status TakeHeapSnapshot(scoped_ptr<base::Value>* snapshot) override;

 private:
  std::string id_;
};

#endif  // XWALK_TEST_XWALKDRIVER_XWALK_STUB_WEB_VIEW_H_
