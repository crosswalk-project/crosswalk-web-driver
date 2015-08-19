#!/usr/bin/env python
# Copyright 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Runs all XwalkDriver end to end tests."""

import optparse
import os
import platform
import shutil
import sys
import tempfile
import traceback

_THIS_DIR = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, os.path.join(_THIS_DIR, os.pardir))

import archive
import xwalk_paths
import util

sys.path.insert(0, os.path.join(xwalk_paths.GetSrc(), 'build', 'android'))
from pylib import constants


def _GenerateTestCommand(script,
                         xwalkdriver,
                         ref_xwalkdriver=None,
                         xwalk=None,
                         xwalk_version=None,
                         android_package=None,
                         verbose=False):
  _, log_path = tempfile.mkstemp(prefix='xwalkdriver_')
  print 'xwalkdriver server log: %s' % log_path
  cmd = [
      sys.executable,
      os.path.join(_THIS_DIR, script),
      '--xwalkdriver=%s' % xwalkdriver,
      '--log-path=%s' % log_path,
  ]
  if ref_xwalkdriver:
    cmd.append('--reference-xwalkdriver=' + ref_xwalkdriver)

  if xwalk:
    cmd.append('--xwalk=' + xwalk)

  if xwalk_version:
    cmd.append('--xwalk-version=' + xwalk_version)

  if verbose:
    cmd.append('--verbose')

  if android_package:
    cmd = ['xvfb-run', '-a'] + cmd
    cmd.append('--android-package=' + android_package)
  return cmd


def RunPythonTests(xwalkdriver, ref_xwalkdriver,
                   xwalk=None, xwalk_version=None,
                   xwalk_version_name=None, android_package=None):
  version_info = ''
  if xwalk_version_name:
    version_info = '(%s)' % xwalk_version_name
  util.MarkBuildStepStart('python_tests%s' % version_info)
  code = util.RunCommand(
      _GenerateTestCommand('run_py_tests.py',
                           xwalkdriver,
                           ref_xwalkdriver=ref_xwalkdriver,
                           xwalk=xwalk,
                           xwalk_version=xwalk_version,
                           android_package=android_package))
  if code:
    util.MarkBuildStepError()
  return code


def RunJavaTests(xwalkdriver, xwalk=None, xwalk_version=None,
                 xwalk_version_name=None, android_package=None,
                 verbose=False):
  version_info = ''
  if xwalk_version_name:
    version_info = '(%s)' % xwalk_version_name
  util.MarkBuildStepStart('java_tests%s' % version_info)
  code = util.RunCommand(
      _GenerateTestCommand('run_java_tests.py',
                           xwalkdriver,
                           ref_xwalkdriver=None,
                           xwalk=xwalk,
                           xwalk_version=xwalk_version,
                           android_package=android_package,
                           verbose=verbose))
  if code:
    util.MarkBuildStepError()
  return code


def RunCppTests(cpp_tests):
  util.MarkBuildStepStart('xwalkdriver_tests')
  code = util.RunCommand([cpp_tests])
  if code:
    util.MarkBuildStepError()
  return code


def DownloadXwalk(version_name, revision, download_site):
  util.MarkBuildStepStart('download %s' % version_name)
  try:
    temp_dir = util.MakeTempDir()
    return (temp_dir, archive.DownloadXwalk(revision, temp_dir, download_site))
  except Exception:
    traceback.print_exc()
    util.AddBuildStepText('Skip Java and Python tests')
    util.MarkBuildStepError()
    return (None, None)


def _KillXwalks():
  xwalk_map = {
      'win': 'xwalk.exe',
      'mac': 'Chromium',
      'linux': 'xwalk',
  }
  if util.IsWindows():
    cmd = ['taskkill', '/F', '/IM']
  else:
    cmd = ['killall', '-9']
  cmd.append(xwalk_map[util.GetPlatformName()])
  util.RunCommand(cmd)


def main():
  parser = optparse.OptionParser()
  parser.add_option(
      '', '--android-packages',
      help='Comma separated list of application package names, '
           'if running tests on Android.')
  # Option 'xwalk-version' is for desktop only.
  parser.add_option(
      '', '--xwalk-version',
      help='Version of xwalk, e.g., \'HEAD\', \'27\', or \'26\'.'
           'Default is to run tests against all of these versions.'
           'Notice: this option only applies to desktop.')
  options, _ = parser.parse_args()

  exe_postfix = ''
  if util.IsWindows():
    exe_postfix = '.exe'
  cpp_tests_name = 'xwalkdriver_tests' + exe_postfix
  server_name = 'xwalkdriver' + exe_postfix

  required_build_outputs = [server_name]
  if not options.android_packages:
    required_build_outputs += [cpp_tests_name]
  try:
    build_dir = xwalk_paths.GetBuildDir(required_build_outputs)
  except RuntimeError:
    util.MarkBuildStepStart('check required binaries')
    traceback.print_exc()
    util.MarkBuildStepError()
  constants.SetBuildType(os.path.basename(build_dir))
  print 'Using build outputs from', build_dir

  xwalkdriver = os.path.join(build_dir, server_name)
  platform_name = util.GetPlatformName()
  if util.IsLinux() and platform.architecture()[0] == '64bit':
    platform_name += '64'
  ref_xwalkdriver = os.path.join(
      xwalk_paths.GetSrc(),
      'xwalk', 'test', 'xwalkdriver', 'third_party', 'java_tests',
      'reference_builds',
      'xwalkdriver_%s%s' % (platform_name, exe_postfix))

  if options.android_packages:
    os.environ['PATH'] += os.pathsep + os.path.join(
        _THIS_DIR, os.pardir, 'xwalk')
    code = 0
    for package in options.android_packages.split(','):
      code1 = RunPythonTests(xwalkdriver,
                             ref_xwalkdriver,
                             xwalk_version_name=package,
                             android_package=package)
      code2 = RunJavaTests(xwalkdriver,
                           xwalk_version_name=package,
                           android_package=package,
                           verbose=True)
      code = code or code1 or code2
    return code
  else:
    latest_snapshot_revision = archive.GetLatestSnapshotVersion()
    versions = [
        ['HEAD', latest_snapshot_revision],
        ['44', archive.CHROME_44_REVISION],
        ['43', archive.CHROME_43_REVISION],
        ['42', archive.CHROME_42_REVISION],
    ]
    code = 0
    for version in versions:
      if options.xwalk_version and version[0] != options.xwalk_version:
        continue
      download_site = archive.Site.CONTINUOUS
      version_name = version[0]
      if version_name == 'HEAD':
        version_name = version[1]
        download_site = archive.GetSnapshotDownloadSite()
      temp_dir, xwalk_path = DownloadXwalk(version_name, version[1],
                                             download_site)
      if not xwalk_path:
        code = 1
        continue
      code1 = RunPythonTests(xwalkdriver,
                             ref_xwalkdriver,
                             xwalk=xwalk_path,
                             xwalk_version=version[0],
                             xwalk_version_name='v%s' % version_name)
      code2 = RunJavaTests(xwalkdriver, xwalk=xwalk_path,
                           xwalk_version=version[0],
                           xwalk_version_name='v%s' % version_name)
      code = code or code1 or code2
      _KillXwalks()
      shutil.rmtree(temp_dir)
    cpp_tests = os.path.join(build_dir, cpp_tests_name)
    return RunCppTests(cpp_tests) or code


if __name__ == '__main__':
  sys.exit(main())
