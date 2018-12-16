# SerialPortFinder

In this repo there are some code snippets to help finding serial port names of usb to serial converters on Windows&reg; machines. 

On Windows&reg; operating systems, the main source of informations of currently opened COM ports, and of COM port to wich a certain USB converters has been attached to, is the **Registry**, and, more precisely, the following **KEYS/KEY GROUP** under the HKLM (HKEY_LOCAL_MACHINE) group:

- SYSTEM\\CurrentControlSet\\Enum\\USB;
- HARDWARE\\DEVICEMAP\\SERIALCOMM;

A typical use is when a software has to read some data from a USB device emulating a COM port (e.g. an Arduino&reg; sending temperature and humidity read from a **DHT11** sensor). Changing the USB port will tipically result in a COM port number change. To avoid the necessity to change it in a configuration file, or even worst, recompiling the software.

There are meny different ways to achieve this goal, and many article, blog posts suggesting good solutions for different programming languages.

In this repo, I'll begin with three of the programming languages I daily use, **C++** and **C#**, then a project for TypeScript/JavaScript will follow.

## C++

SerialPortFinder is the C++ project 

## C#