#include <stdio.h>
#include <string.h>

#include "rf21x-wrap.h"

#ifdef WIN32
    #include <windows.h>
    #define RF21X_SLEEP(n) (Sleep(n))
#else
    #include <unistd.h>
    #define RF21X_SLEEP(n) (usleep(1000*n))
#endif

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Name: rf21x-test-------------- \n");
        printf("Usage: rf21x-test DeviceType PortPath \n");
        printf("  Usage Example: rf21x-test rf217 hid:// \n");
        printf("  Usage Example: rf21x-test rf217 hid://0007800D \n");
        printf("  Usage Example: rf21x-test rf217 serial:///dev/ttyUSB0 \n");
        printf("  Usage Example: rf21x-test rf217 tcp://192.168.1.10:9000+hid:// \n");
        printf("Parameters:\n");
        printf("  DeviceType: rf217, rf218, rf219, rf215. \n");
        printf("  PortPath: PortType://PortAddress. \n");
        printf("    PortType: hid, serial, tcp \n");
        printf("    PortAddress: port address \n");
        printf("************************************************************\n");
        printf("Press any key to exit.\n");
        while (1)
        {
            if (getchar() > 0)
            {
                break;
            }
        }
        return 0;
    }

    // User can use serial number as address like "hid://0007800D" to select a hid device to be opened.
    printf("Start listing all current HID device:\n");
    std::vector<std::string> snVector = rf21x::getHidSerialNumber();
    for (size_t i = 0; i < snVector.size(); ++i)
    {
        std::string sn = snVector[i];
        printf("HID Device %d: %s\n", i, sn.c_str());
    }
    printf("Finish listing all current HID device.\n\n");

#ifdef _WIN32
    snVector = rf21x::getHidTempPath();
    std::string tempPath;
    if (snVector.empty() == false)
    {
        tempPath = snVector[0];
    }
#endif

    char *typeName = argv[1];
    int type = RF21X_DT_Unknow;
    if (strcmp(typeName, "rf217") == 0)
    {
        type = RF21X_DT_RF217;
    }
    else if (strcmp(typeName, "rf218") == 0)
    {
        type = RF21X_DT_RF218;
    }
    else if (strcmp(typeName, "rf219") == 0)
    {
        type = RF21X_DT_RF219;
    }
    else if (strcmp(typeName, "rf215") == 0)
	{
		type = RF21X_DT_RF215;
	}
	else
    {
        printf("DeviceType error.\n");
    }

    rf21x::RF21xDevice rf21x;

//    std::string sn = "";
//    if (rf21x.readDeviceSerialNumber(type, argv[2], sn, true) == false)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(default): %s \n", sn.c_str());
//    }

//    if (rf21x.readDeviceSerialNumber(type, argv[2], sn, false) == false)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(user defined): %s \n", sn.c_str());
//    }

//    if (sn.length() > 0)
//    {
//        sn[0] = '3';
//    }
//    if (rf21x.writeDeviceSerialNumber(type, argv[2], sn) == false)
//    {
//        printf("fail to writeDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("writeDeviceSerialNumber: %s \n", sn.c_str());
//    }

//    if (rf21x.readDeviceSerialNumber(type, argv[2], sn, false) == false)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(user defined): %s \n", sn.c_str());
//    }

    if (rf21x.open(type, argv[2], 1, 40))
    {
        printf("Success to open device.\n");
        RF21X_SLEEP(2000);
    }
    else
    {
        printf("Fail to open device.\n");
        return 0;
    }

    if (rf21x.startQuiz(RF21X_QT_Single, 2, 10))
    {
        printf("Success to start a quiz.\n");
    }
    else
    {
        printf("Fail to start a quiz.\n");
        return 0;
    }

    printf("Getting message from device, press Ctrl+C to break...\n");
    rf21x::RF21xMessage message;
    message.data = "Init\n"; // This is used for fixing a strange bug in some android system that cause "segement fault".
    while (1)
    {
#ifdef _WIN32
        std::string path = argv[2];
        if (path.size() >= 6 && path.compare(0, 6, "hid://") == 0
                && rf21x::checkHidCable(tempPath) == false)
        {
            printf("Cable error! \n");
        }
#endif
        if (rf21x.getMessage(message))
        {
            if (message.messageType == RF21X_MT_Teacher)
            {
                printf("Teacher: %s\n", message.data.c_str());
            }
            else if (message.messageType == RF21X_MT_Student)
            {
                printf("Student %d for current question: %s\n", message.keypadId, message.data.c_str());
            }
            else if (message.messageType == RF21X_MT_SetId)
            {
                printf("Set ID %d\n", message.keypadId);
            }
            printf("Raw data: ");
            for (std::vector<unsigned char>::const_iterator i = message.rawData.begin(); i != message.rawData.end(); ++i)
            //for (auto i = message.rawData.begin(); i != message.rawData.end(); ++i)
            {
                printf("%02X ", *i);
            }
            printf("\n");
        }
        else
        {
            RF21X_SLEEP(100);
        }
    }
    printf("Press any key to stop.\n");
    getchar();
    printf("Success Stop quiz and close device.\n");
    rf21x.stopQuiz();
    rf21x.close();

    return 0;
}
