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
>>> driver = xwalkdriver.XwalkDriver('http://127.0.0.1:9515')
>>> driver.Load('http://www.google.com')
>>> driver.Quit()
>>> cd_server.Kill()

For local APP in Linux/Deepin

(1) Download an executable binary from https://github.com/VicoandMe/XwalkDriver

(2) Add --remote-debugging-port behind the 'exec' command in your local APP. 
   e.g I packaged an example named 'foo'. And I installed 'foo' in /usr/bin
       vim /usr/bin/foo

       add --remote-debugging-port in the last line:
       exec xwalk "${BASEDIR}/www/manifest.json" --remote-debugging-port=12450

(3) Use following python instructions to do a basic test.

$ export PYTHONPATH=<THIS_DIR>/server:<THIS_DIR>/client
$ python
>>> import server
>>> import xwalkdriver
>>> cd_server = server.Server('/path/to/xwalkdriver/executable')
>>> driver = xwalkdriver.XwalkDriver('http://127.0.0.1:9515', xwalk_binary = '/usr/bin/foo', debug_port = '12450')
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

(5) Run xwalkdriver binary.
    $./xwalkdriver
    If xwalkdriver runs on a remote server, you can authorize security clients on where

(6) Execute following commands to test:
$ python
>>> import xwalkdriver
>>> driver = xwalkdriver.XwalkDriver('http://127.0.0.1:9515', android_package='', android_activity='')
>>> driver.quit()

Use selenium to do TEST
(1) Download an executable binary from https://github.com/VicoandMe/XwalkDriver

(2) Add --remote-debugging-port behind the 'exec' command in your local APP.    
  e.g I packaged an example named 'foo'. And I installed 'foo' in /usr/bin        
      vim /usr/bin/foo
      add --remote-debugging-port in the last line:
        exec xwalk "${BASEDIR}/www/manifest.json" --remote-debugging-port=12450
(3) Use following python instructions to do a basic test.

>>>import time
>>>from selenium.webdriver.remote.webdriver import WebDriver
>>>capabilities = {'loggingPrefs': {}, 
	           'xwalkOptions': {
	             'binary': '/usr/bin/foo', 
		     'debugPort': '12450'
	             }
	          }
>>>driver = WebDriver('http://127.0.0.1:9515',desired_capabilities = capabilities,keep_alive=True)
>>>driver.get('http://www.baidu.com')
>>>search_box = driver.find_element_by_name('wd')
>>>search_box.send_keys('WebDriver')
>>>search_box.submit()







 
	

