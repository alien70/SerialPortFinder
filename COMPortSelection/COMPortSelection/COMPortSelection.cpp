// COMPortSelection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "SerialPortFinder.hpp"

string hardwareId = "VID_2341&PID_003E&MI_00";

int main(int argc, char **argv)
{
	if (argc > 1) {
		hardwareId = argv[argc - 1];
	}

	string comPortName = Utilities::IO::SerialPortFinder::GetComPortNameByHardwareId(hardwareId);

	stringstream ss;

	ss << "The Controller with "
		<< hardwareId
		<< " hardware Id is ";

	if (comPortName.length() > 0) {
		ss << "connected to port: " << comPortName;
	}
	else
	{
		ss << "not connected";
	}

	cout << "=============================================================" << endl;
	
	cout << ss.str() << endl;

	cout << "=============================================================" << endl;

	return 0;
}