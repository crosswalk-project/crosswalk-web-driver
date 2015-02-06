Crosswalk Web Driver
====================

## Introduction

Crosswalk WebDriver is an implementation of the WebDriver standard.
Part of code is ported from ChromeDriver.

It is an open source project started by the Intel Open Source Technology Center
(http://www.01.org)

## Directory Structure

1. `xwalkdriver/` is source code.
2. If you want to use xwalkdriver binary, please try `./bin/xwalkdriver64_release`.
   And `./bin/xwalkdriver32_release` is used for 32-bit Linux PC.
3. If you want to build the project, please follow the first two steps:
  1. Patch the `xwalk_test.gypi` to crosswalk-project `crosswalk/xwalk_test.gypi`.
  2. Copy the whole directory `xwalkdriver/` to crosswalk-project `crosswalk/test/`.
  3. If you want to build for Linux32, please run the follow commands:
     ```
     $ export GYP_GENERATORS='ninja'
     $ GYP_DEFINES="target_arch=ia32 clang=0" xwalk/gyp_xwalk
     $ ninja -C out/Release xwalkdriver
     $ ninja -C out/Release xwalkdriver_unittests(optional)
     ```

  4. If you want to build for Linux64, please run the follow commands:
     ```
     $ export GYP_GENERATORS='ninja'
     $ GYP_DEFINES="target_arch=x64 clang=1" xwalk/gyp_xwalk
     $ ninja -C out/Release xwalkdriver
     $ ninja -C out/Release xwalkdriver_unittests(optional)
     ```

     For more build info, see `xwalkdriver/README.txt`, or
     https://crosswalk-project.org/#contribute/building_crosswalk

## Documents

For Cosswalk WebDriver usage, please check `xwalkdriver/README.txt` and
[Wiki](https://crosswalk-project.org/#wiki/Crosswalk-WebDriver)

## Community

Any questions of Cosswalk WebDriver usage, please ask mailing list:
https://lists.crosswalk-project.org/mailman/listinfo/crosswalk-help

To improve Crosswalk WebDriver, please go here:
https://lists.crosswalk-project.org/mailman/listinfo/crosswalk-dev

You can also join us on IRC : #crosswalk on Freenode

## License

Crosswalk's code uses the BSD license, see our `LICENSE` file.
