using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace testGsp
{
    class Program
    {
        static GspDevice sp = new GspDevice();

        static bool rf218_sendCommand(GspData cmd)
        {
            if (sp.isOpened() == false)
            {
                return false;
            }

            bool isOk = true;

            GspData c = cmd;
            byte v = 0;
            for (int i = 0; i < cmd.Count; ++i)
            {
                byte d = cmd[i];
                v = (byte)(v ^ d);
            }
            c.Add(v);

            if (sp.writeData(c) != cmd.Count + 1)
            {
                isOk = false;
                goto finish;
            }

        finish:
            return isOk;
        }

        static void Main(string[] args)
        {
            Console.WriteLine("RF21x started...");

            GspHidSerialNumbers sn = GspDevice.getHidSerialNumber();
            for (int i = 0; i < sn.Count; ++i)
            {
                sn[i] = String.Format("hid://{0}", sn[i]);
                Console.Write("HID Device {0}: <{1}>\n", i, sn[i]);
            }
            Console.Write("Finish listing all current HID device.\n\n");

            if (sn.Count < 1)
            {
                Console.Write("No HID device.\n");
                return;
            }

            if (sp.open("hid://0007800C", 115200) == false)
            {
                Console.Write("Faild open hid device.\n");
            }
            else
            {
                Console.Write("Success open hid device.\n");
            }

            int maxId = 40;

            GspData cmd = new GspData();
            GspData ret = new GspData();

            cmd.Add((byte)(0x70 | (maxId % 0x10)));
            cmd.Add((byte)(0x80 | (maxId / 0x10)));
            Program.rf218_sendCommand(cmd);
            System.Threading.Thread.Sleep(300);
            sp.readData(ret, 5);
            for (int i = 0; i < ret.Count; ++i)
            {
                Console.Write("{0:X2} ", ret[i]);
            }
            Console.Write("\n");
            sp.flushReciveFIFO();

            cmd.Add(0x5A);
            cmd.Add(0x80);
            Program.rf218_sendCommand(cmd);
            System.Threading.Thread.Sleep(300);
            sp.readData(ret, 5);
            for (int i = 0; i < ret.Count; ++i)
            {
                Console.Write("{0:X2} ", ret[i]);
            }
            Console.Write("\n");
            sp.flushReciveFIFO();


            Console.Write("Getting message from rf218 device, press Ctrl+C to break...\n");
            while (true)
            {
                System.Threading.Thread.Sleep(300);
                sp.readData(ret, 1000);
                {
                    for (int i = 0; i < ret.Count; ++i)
                    {
                        Console.Write("{0:X2} ", ret[i]);
                    }
                }
            }
            
        }
    }
}
