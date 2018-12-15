/*
 * Author: Maurizio Attanasi
 * Created on Tue Dec 11 2018
 *
 * Copyright (c) 2018 Memento S.r.l.
 */

#include <sstream>

#include "SerialPortFinder.hpp"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

using namespace std;

const wstring USB_ENUM_KEY = L"SYSTEM\\CurrentControlSet\\Enum\\USB";
const wstring SERIALCOMM_KEY = L"HARDWARE\\DEVICEMAP\\SERIALCOMM";

namespace Utilities
{
	namespace IO
	{
		string SerialPortFinder::GetComPortNameByHardwareId(const string & id)
		{
			vector<string> serialComms = GetConnectedSerialPortNames();

			vector<string> usbKeysByVendorId = GetUsbKeysByHardwareId(id);

			vector<string>::iterator it;
			for (it = usbKeysByVendorId.begin(); it != usbKeysByVendorId.end(); ++it) {
				string portName = GetPortNameByKeyName(id, *it);

				vector<string>::iterator it1 = find(serialComms.begin(), serialComms.end(), portName);
				if (it1 != serialComms.end()) {
					return *it1;
				}
			}

			return string();
		}

		vector<string> SerialPortFinder::GetConnectedSerialPortNames()
		{
			HKEY hSerialCommsKey;
			vector<string> serialPortNames;

			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				SERIALCOMM_KEY.c_str(),
				0,
				KEY_READ,
				&hSerialCommsKey) == ERROR_SUCCESS
				)
			{
				serialPortNames = QueryKey(hSerialCommsKey);
			}

			RegCloseKey(hSerialCommsKey);

			return serialPortNames;
		}

		vector<string> SerialPortFinder::GetUsbKeysByHardwareId(const string & vendorId)
		{
			HKEY hUSBsKeys;
			vector<string> subKeys;

			std::wostringstream w;
			wstring _id(vendorId.begin(), vendorId.end());

			w << USB_ENUM_KEY << L"\\" << _id;

			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				w.str().c_str(),
				0,
				KEY_READ,
				&hUSBsKeys) == ERROR_SUCCESS
				)
			{
				subKeys = QueryKey(hUSBsKeys, false);
			}

			RegCloseKey(hUSBsKeys);

			return subKeys;
		}

		string SerialPortFinder::GetPortNameByKeyName(const string& hardwareId, const string & keyName)
		{
			HKEY hPortKey;

			std::wostringstream w;
			wstring _vendorId(hardwareId.begin(), hardwareId.end());
			wstring _keyName(keyName.begin(), keyName.end());

			w << USB_ENUM_KEY << L"\\" << _vendorId << L"\\" << _keyName << L"\\Device Parameters";

			string retVal;

			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				w.str().c_str(),
				0,
				KEY_READ,
				&hPortKey) == ERROR_SUCCESS
				)
			{
				TCHAR buf[255] = { 0 };
				DWORD dwBufSize = sizeof(buf);

				if (RegQueryValueEx(hPortKey, TEXT("PortName"), NULL, NULL, (LPBYTE)buf, &dwBufSize) == ERROR_SUCCESS) {
					wstring val(&buf[0]);

					retVal = string(val.begin(), val.end());
				}
			}

			RegCloseKey(hPortKey);

			return retVal;
		}

		vector<string> SerialPortFinder::QueryKey(HKEY hKey, bool queryValues)
		{
			TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
			DWORD    cbName;                   // size of name string 
			TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
			DWORD    cchClassName = MAX_PATH;  // size of class string 
			DWORD    cSubKeys = 0;               // number of subkeys 
			DWORD    cbMaxSubKey;              // longest subkey size 
			DWORD    cchMaxClass;              // longest class string 
			DWORD    cValues;              // number of values for key 
			DWORD    cchMaxValue;          // longest value name 
			DWORD    cbMaxValueData;       // longest value data 
			DWORD    cbSecurityDescriptor; // size of security descriptor 
			FILETIME ftLastWriteTime;      // last write time 

			DWORD i, retCode;

			TCHAR  achValue[MAX_VALUE_NAME];
			DWORD cchValue = MAX_VALUE_NAME;

			// Get the class name and the value count. 
			retCode = RegQueryInfoKey(
				hKey,                    // key handle 
				achClass,                // buffer for class name 
				&cchClassName,           // size of class string 
				NULL,                    // reserved 
				&cSubKeys,               // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				&ftLastWriteTime);       // last write time 

			// Enumerate the subkeys, until RegEnumKeyEx fails.

			vector<string> retVal;

			if (!queryValues && cSubKeys)
			{
				for (i = 0; i < cSubKeys; i++)
				{
					cbName = MAX_KEY_LENGTH;
					retCode = RegEnumKeyEx(hKey, i,
						achKey,
						&cbName,
						NULL,
						NULL,
						NULL,
						&ftLastWriteTime);
					if (retCode == ERROR_SUCCESS)
					{
						wstring k(&achKey[0]);
						string key(k.begin(), k.end());

						retVal.push_back(key);
						/*_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);*/
					}
				}
			}

			// Enumerate the key values. 

			if (queryValues && cValues)
			{
				for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
				{
					cchValue = MAX_VALUE_NAME;
					achValue[0] = '\0';
					retCode = RegEnumValue(hKey, i,
						achValue,
						&cchValue,
						NULL,
						NULL,
						NULL,
						NULL);

					if (retCode == ERROR_SUCCESS)
					{
						TCHAR buf[255] = { 0 };
						DWORD dwBufSize = sizeof(buf);
						DWORD dwType = REG_SZ;

						if (ERROR_SUCCESS == RegQueryValueEx(hKey, achValue, 0, &dwType, (LPBYTE)buf, &dwBufSize)) {

							wstring val(&buf[0]);

							string v(val.begin(), val.end());
							retVal.push_back(v);

						}
					}
				}
			}

			return retVal;
		}

	}
}