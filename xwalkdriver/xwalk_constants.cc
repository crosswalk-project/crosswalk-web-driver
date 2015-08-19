// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk_constants.h"

#include "base/files/file_path.h"

#define FPL FILE_PATH_LITERAL

#if defined(OS_MACOSX)
#define CHROMIUM_PRODUCT_STRING "Chromium"
#if defined(GOOGLE_CHROME_BUILD)
#define PRODUCT_STRING "Google Xwalk"
#elif defined(CHROMIUM_BUILD)
#define PRODUCT_STRING "Chromium"
#else
#error Unknown branding
#endif
#endif  // defined(OS_MACOSX)

namespace xwalk {
const char kInitialProfile[] = "Default";
const base::FilePath::CharType kPreferencesFilename[] = FPL("Preferences");
const base::FilePath::CharType kLocalStateFilename[] = FPL("Local State");
const base::FilePath::CharType kFirstRunSentinel[] = FPL("First Run");
}  // namespace xwalk

#undef FPL
