/*
 * Author: Maurizio Attanasi
 * Created on Tue Dec 11 2018
 *
 * Copyright (c) 2018 Memento S.r.l.
 */

#pragma once

#include <windows.h>
#include <string>
#include<vector>

using namespace std;

namespace Utilities
{
	namespace IO
	{
		/**
		 * @brief	Utility class for Finding Serial Port names 
		 * 			when using virtual USB to COM Port Converters (such as FTDI https://www.ftdichip.com/Drivers/VCP.html)
		 */
		class SerialPortFinder
		{
			// Methods
		public:
			/**
			 * @brief Get the Com Port Name By Hardware Id
			 *
			 * @param id:		The unique hardware id of the usb to serial port device
			 * @return string:	If connected, returns the port name to which the
			 *					device is connected, otherwise an empty string.
			 */
			static string GetComPortNameByHardwareId(const string& id);

		private:
			/**
			 * @brief Returns the names of the currently occupied serial ports
			 *
			 * @return vector<string>
			 */
			static vector<string> GetConnectedSerialPortNames();

			/**
			 * @brief Gets the registry subkeys related to a given hardware id
			 *
			 * @param id: The unique hardware id of the usb to serial port device
			 * @return vector<string>
			 */
			static vector<string> GetUsbKeysByHardwareId(const string & id);

			/**
			 * @brief Get the Port Name from the hardware id, subkey pair
			 *
			 * @param hardwareId:	The unique hardware id of the usb to serial port device
			 * @param keyName:		One of the registry subkeys found with the GetUsbKeysByHardwareId
			 *						function
			 * @return string
			 */
			static string GetPortNameByKeyName(const string& hardwareId, const string& keyName);

			/**
			 * @brief Queries the registry for subkeys or values
			 *
			 * @param hKey:				The handle to an opened registry key
			 * @param queryValues		If true it returns the values (if any) of the given key
			 *							the subkeys otherwise
			 * @return vector<string>
			 */
			static vector<string> QueryKey(HKEY hKey, bool queryValues = true);

		};
	}
}