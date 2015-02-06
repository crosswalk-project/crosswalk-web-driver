This file contains high-level info about how XwalkDriver works.

XwalkDriver is an implementation of the WebDriver standard. Part of code is
ported from ChromeDriver.

=====How To=====
For Linux xwalk:
(1) Build XwalkDriver by building the 'xwalkdriver' target and get an executable
binary in the build folder named 'xwalkdriver'.

(2) Build 'xwalk' target, install it in "/opt/crosswalk" or working path of
'xwalkdriver'.

(3) Use following python instructions to do a basic test.

$ export PYTHONPATH=<THIS_DIR>/server:<THIS_DIR>/client
$ python
>>> import server
>>> import xwalkdriver
>>> cd_server = server.Server('/path/to/xwalkdriver/executable')
>>> driver = xwalkdriver.XwalkDriver(cd_server.GetUrl())
>>> driver.Load('http://www.google.com')
>>> driver.Quit()
>>> cd_server.Kill()

For Android xwalk:

(1) Build XwalkDriver by building the 'xwalkdriver' target and get an executable
binary in the build folder named 'xwalkdriver'(Details referred to ../README.md).
Or download the binary from
    https://github.com/crosswalk-project/crosswalk-web-driver/bin

(2) Pakage your app by execute command
    python make_apk.py --package=YOUR_APP_PACKAGE_NAME --manifest=YOUR_APP_PATH/manifest.json \
      --arch=YOUR_DEVICE_ARCH --enable-remote-debugging

(3) Install your apk to device.

(4) Install Selenium package by executing command in sudo mode
    apt-get install python-pip && pip install selenium

(5) Run xwalkdriver binary.
    $./xwalkdriver
    If xwalkdriver runs on a remote server, you can authorize security clients on where
test suit executes by white-list-ip like this.
    $./xwalkdriver --whitelisted-ips=client-ip;

(6) Execute following commands to test:
$ python
>>> from selenium import webdriver
>>>
capabilities = {
  'xwalkOptions': {
    'androidPackage': 'YOUR_PACKAGE_NAME', 
    'androidActivity': '.YOUR_ACTIVITY_NAME',
    'adb-port': 5037(default option if not selected),
  }
}
>>> driver = webdriver.Remote('http://localhost:9515', capabilities)      
>>> driver.save_screenshot("screenshot.png")
>>> driver.quit()

    
For Tizen xwalk:
(1) Check status of sdbd_tcp.service on Tizen IVI, start it if neecessary:
systemctl status sdbd_tcp
systemctl start sdbd_tcp

(2) Connect Tizen IVI and PC with sdb
    a). Run the command on PC: han@hanlab:~$ sdb connect <tizen ivi ip>
    b). Run the command on PC: han@hanlab:~$ sdb root on
    c). Run the command on PC: han@hanlab:~$ sdb push webapi.xpk /home/app

(3) Install and launch the xwalk as server mode on Tizen IVI:
su - app
systemctl --user status xwalk
pkgcmd -i -t xpk -q -p /home/app/webapi.xpk

(4) Set remote debug port by insert "--remote-debugging-port=9222" into "/usr/lib/systemd/user/xwalk.service" on Tizen IVI.

(5) Restart the xwalk.service on Tizen IVI:
systemctl --user daemon-reload
systemctl --user restart xwalk

(6) If Selenium package not installed on PC. Install Selenium package by executing command
    apt-get install python-pip && pip install selenium

(7) Run xwalkdriver binary on PC with command:
    $./xwalkdriver
    If xwalkdriver runs on a remote server, you can authorize security clients on where
test suit executes by white-list-ip like this.
    $./xwalkdriver --whitelisted-ips=client-ip;

(8) Execute following commands to test(Note to replace your tizen application Id):
$ python
>>> from selenium import webdriver
>>> 
capabilities = {
  'xwalkOptions': {
    'tizenDebuggerAddress': '10.238.158.97:9222',
    'tizenAppId': 'xwalk.ihogjblnkegkonaaaddobhoncmdpbomi',
    'sdb-port': 26099(default option if not selected),
  }
}

>>> driver = webdriver.Remote('http://localhost:9515', capabilities)
>>> driver.title
>>> driver.save_screenshot("screenshottest.png")
>>> driver.quit()


 
