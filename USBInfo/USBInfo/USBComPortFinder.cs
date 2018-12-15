using System;
using System.Collections.Generic;
using System.Management;

namespace USBInfo
{
    class USBComPortFinder
    {
        public static string FindPortByDescription(string description)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Description"].ToString().ToLower().Equals(description.ToLower()))
                    {
                        string comName = ParseCOMName(obj);
                        if (!string.IsNullOrEmpty(comName))
                            return comName;
                    }
                }
                catch (Exception)
                {
                    //Console.WriteLine(e);
                }
            }

            return null;
        }

        public static string FindPortByHardwareId(string hardwareId)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    string[] ids = (string[])obj["HardwareID"];

                    foreach (var item in ids)
                    {
                        if (string.IsNullOrEmpty(item))
                            continue;

                        if (item.ToLower().Contains(hardwareId.ToLower()))
                        {
                            string comName = ParseCOMName(obj);
                            if (!string.IsNullOrEmpty(comName))
                                return comName;
                        }
                    }
                }
                catch (Exception)
                {
                    //Console.WriteLine(e);
                }
            }

            return null;
        }

        public static string FindPortByManufacturer(string manufacturer)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Manufacturer"].ToString().ToLower().Equals(manufacturer.ToLower()))
                    {
                        string comName = ParseCOMName(obj);
                        if (!string.IsNullOrEmpty(comName))
                            return comName;
                    }
                }
                catch (Exception)
                {
                    //Console.WriteLine(e);
                }
            }

            return null;
        }

        static string ParseCOMName(ManagementObject obj)
        {
            string name = obj["name"].ToString();
            int startIndex = name.LastIndexOf("(");
            int endIndex = name.LastIndexOf(")");

            if (startIndex != -1 && endIndex != -1)
                return name.Substring(startIndex + 1, endIndex - startIndex - 1);

            return null;
        }

        static string[] FinAllPorts()
        {
            List<string> ports = new List<string>();

            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Caption"].ToString().Contains("(COM"))
                    {
                        string commName = ParseCOMName(obj);
                        if (!string.IsNullOrEmpty(commName))
                        {
                            ports.Add(commName);
                        }
                    }
                }
                catch (Exception)
                {
                    //Console.WriteLine(e);
                }
            }

            return ports.ToArray();
        }

        static ManagementObject[] FindPorts()
        {
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher(@"root\CIMV2", "SELECT * FROM Win32_PnpEntity");
                List<ManagementObject> objects = new List<ManagementObject>();

                foreach (ManagementObject obj in searcher.Get())
                {
                    objects.Add(obj);
                }

                return objects.ToArray();
            }
            catch (Exception)
            {
                //Console.WriteLine(e);

                return new ManagementObject[] { };
            }
        }
    }
}
