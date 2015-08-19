// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// A handful of resource-like constants related to the Xwalk application.

#ifndef CHROME_COMMON_CHROME_CONSTANTS_H_
#define CHROME_COMMON_CHROME_CONSTANTS_H_

#include "base/files/file_path.h"

#if defined(OS_WIN)
#if defined(GOOGLE_CHROME_BUILD)
#define PRODUCT_STRING_PATH L"Google\\Xwalk"
#elif defined(CHROMIUM_BUILD)
#define PRODUCT_STRING_PATH L"Chromium"
#else
#error Unknown branding
#endif
#endif  // defined(OS_WIN)

namespace xwalk {
extern const char    kInitialProfile[];
extern const base::FilePath::CharType kFirstRunSentinel[];
extern const base::FilePath::CharType kLocalStateFilename[];
extern const base::FilePath::CharType kPreferencesFilename[];
}  // namespace xwalk

#endif  // CHROME_COMMON_CHROME_CONSTANTS_H_
