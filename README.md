# Finding serial port names of usb to serial converters on window machines

As usual, this is a #noteformyself article on how to find serial port names of USB to serial converters on Windows&reg machines.

On Windows&reg operating systems, the main source of information of currently opened COM ports, and of COM port to which a certain USB converter has been attached to, is the Registry, and, more precisely, the following KEYS/KEY GROUP under the HKLM (HKEY_LOCAL_MACHINE) group:

- SYSTEM\\CurrentControlSet\\Enum\\USB;
- HARDWARE\\DEVICEMAP\\SERIALCOMM;

Typical use is when software has to read some data from a USB device emulating a COM port (e.g. an Arduino&reg sending temperature and humidity read from a DHT11 sensor). Changing the USB port will typically result in a COM port number change.

There are many ways to avoid the necessity to change it in a configuration file, or even worst, recompiling the software, and many articles, blog posts suggesting good solutions for different programming languages.

I'll begin with three of the programming languages I daily use, **C++** and **C#**, then a project for TypeScript/JavaScript will follow.

## C++ Demo

[SerialPortFinder](https://github.com/alien70/SerialPortFinder/tree/master/COMPortSelection) is the C++ project 

## C# Demo

[USBInfo](https://github.com/alien70/SerialPortFinder/tree/master/USBInfo) is a .NET/C# project.

All the code can be found on my github repo at the following [link](https://github.com/alien70/SerialPortFinder).

Enjoy