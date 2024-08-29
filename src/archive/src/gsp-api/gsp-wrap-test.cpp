#include <stdio.h>
#include <string.h>

#include "gsp-wrap.h"

#ifdef WIN32
    #include <windows.h>
    #define GSP_SLEEP(n) (Sleep(n))
#else
    #include <unistd.h>
    #define GSP_SLEEP(n) (usleep(1000*n))
#endif

static gsp::GspDevice sp;

bool rf218_sendCommand(const std::vector<unsigned char> &cmd)
{
    if (sp.isOpened() == false)
    {
        return false;
    }

    bool isOk = true;

    std::vector<unsigned char> c = cmd;
    unsigned char v = 0;
    for (size_t i = 0; i < cmd.size(); ++i)
    {
        v = v ^ cmd[i];
    }
    c.push_back(v);

    if (sp.writeData(c) != cmd.size() + 1)
    {
        isOk = false;
        goto finish;
    }

finish:
    return isOk;
}



int main(int argc, char **argv)
{
    // User can use serial number as address like "hid://0007800D" to select a hid device to be opened.
    printf("Start listing all current HID device:\n");
    std::vector<std::string> snVector = gsp::GspDevice::getHidSerialNumber();
    for (size_t i = 0; i < snVector.size(); ++i)
    {
        std::string sn = snVector[i];
        printf("HID Device %d: <%s>\n", i, sn.c_str());
    }
    printf("Finish listing all current HID device.\n\n");

    if (snVector.size() < 1)
    {
        printf("No HID device.\n");
        return 0;
    }

    if (sp.open("hid://0007800C", 115200) == false)
    {
        printf("Faild open hid device.\n");
    }
    else
    {
        printf("Success open hid device.\n");
    }


    int maxId = 40;
    int minId = 1;

    std::vector<unsigned char> cmd;
    std::vector<unsigned char> ret;

    cmd.push_back(0x70 | (maxId % 0x10));
    cmd.push_back(0x80 | (minId / 0x10));
    rf218_sendCommand(cmd);
    GSP_SLEEP(300);
    sp.readData(ret, 5);
    for (int i = 0; i < ret.size(); ++i)
    {
        printf("%02X ", ret[i]);
    }
    printf("\n");
    sp.flushReciveFIFO();

    cmd.clear();
    cmd.push_back(0x5A);
    cmd.push_back(0x80);
    rf218_sendCommand(cmd);
    GSP_SLEEP(300);
    sp.readData(ret, 5);
    for (int i = 0; i < ret.size(); ++i)
    {
        printf("%02X ", ret[i]);
    }
    printf("\n");
    sp.flushReciveFIFO();

    printf("Getting message from rf218 device, press Ctrl+C to break...\n");
    while (true)
    {
        GSP_SLEEP(100);
        sp.readData(ret, 1000);
        {
            for (int i = 0; i < ret.size(); ++i)
            {
                printf("%02X ", ret[i]);
            }
        }
    }

    return 0;
}
