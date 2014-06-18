crosswalk-web-driver
====================

## Introduction

Crosswalk WebDriver is an implementation of the WebDriver standard. Part of code is ported from ChromeDriver.
It is an open source project started by the Intel Open Source Technology Center
(http://www.01.org)

## Directory Structure
1 "./xwalkdriver/" is source code.

2 If you want to use xwalkdriver binary. You can use "./bin/xwalkdriver".
  And ./bin/xwalkdriver32 is used for 32bit Linux PC.

3 If you want to build the project. The follow first two step is needed:
  1 You should patch the "xwalk_test.gypi" to crosswalk-project "crosswalk/xwalk_test.gypi".
  2 Copy the "./xwalkdriver/" folder to crosswalk-project. Land in "./crosswalk/test/".
  3 If you want to build for Linux, You can run the follow command:
    $export GYP_GENERATORS='ninja'
    $python xwalk/gyp_xwalk
    $ninja -C out/Release xwalkdriver
    The more build guide, see "./xwalkdriver/README.txt".
    Or https://crosswalk-project.org/#contribute/building_crosswalk.

## Documents
The crosswalk web driver usage, You can find in ./xwalkdriver/README.txt and wiki.
Check out our [Wiki](https://crosswalk-project.org/#wiki/Crosswalk-WebDriver.)

## Community

How to use Crosswalk you can ask on the mailing list : https://lists.crosswalk-project.org/mailman/listinfo/crosswalk-help

Development of Crosswalk : https://lists.crosswalk-project.org/mailman/listinfo/crosswalk-dev

We are also on IRC : #crosswalk on Freenode

## License

Crosswalk's code uses the BSD license, see our `LICENSE` file.
