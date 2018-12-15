using System;

namespace USBInfo
{
    public class Program
    {
        public static void Main()
        {
            string comPort = USBComPortFinder.FindPortByHardwareId("VID_2341&PID_003E&MI_00");
            Console.WriteLine(comPort);
        }


    }
}
