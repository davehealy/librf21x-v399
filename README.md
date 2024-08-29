# librf21x - Enjoy rf21x device SDK
EnjoyARS librf21x SDK is a cross platform SDK for rf21x devices written in C/C++.

## Supported platforms:
* Windows XP, Windows Vista, Windows 7, Windows 8. (Serial Port, HID, Socket)
* Linux with kernel 2.6 or later version. (HID, Socket)
* Mac. (HID)
* Android. (Socket) (See "andoird/README.md" for more details)

## Supported devices type:
* rf217
* rf218
* rf219
* rf215
* rf228

## Supperted device interfaces:
* Serial port
* HID serial port
* Network interface

## Release Package
Different wraps are stored in different directories, but all the wraps need it's C API runtime library. 

## Usage

All the API is thread safe. Basic usage:

* Create device instance: Create a class with a specified device type.
* Open device: Open device with a specified port URL. Teacher keypad will be available after this step, user can get message from device instance.
* Start and stop quiz: Start a quiz. Student keypad will be available after this step, user can get message from device instance.Then you can stop it.
* Close device.
* Destory device instance.
* Port name example:
  * serial port device:
    * "serial://COM1" (Windows)
    * "serial://COM2" (Windows)
    * "serial:///dev/ttyUSB0" (Linux)
    * "serial:///dev/ttyUSB1" (Linux)
  * serial port with HID device:
    * "hid://"
    * "hid://0007800C"
  * network device:
    * "tcp://192.168.1.1:9000+serial://COM1"
    * "tcp://192.168.2.2:9000+hid://"

## How to Build
For new user, you can just follow the step in "doc/Introduction_For_Building.pdf".
Inorder to build librf21x, you need use CMake.
In Linux, libusb-1.0 is needed.
SWIG will be need for other language wraps like Java, Python, CSharp.
Doxygen will be need to build documents.

## Documents
Please see "doc/html/index.html" file for C/C++ API.
For Other language wraps, test examples are helpful.

## Support
- http://enjoy-ars.com
- enjoyars@gmail.com
- miaow1988@qq.com
