#include <stdio.h>
#include <string.h>

#include "rf21x.h"

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
        printf("Name: rf21x-test \n");
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
    printf("Start listing all current HID devices:\n");
    std::string serialNumber;
    int i = 0;
    while (getHidSerialNumber(i, serialNumber))
    {
        printf("HID Device %d: %s\n", i, serialNumber.c_str());
        ++i;
    }
    printf("Finish listing all current HID devices.\n\n");

#ifdef _WIN32
    printf("Start listing all current HID devices by temp path:\n");
    std::string tempPath;
    int j = 0;
    while (getHidTempPath(j, tempPath))
    {
        printf("HID Device %d: %s\n", j, tempPath.c_str());
        ++j;
    }
    printf("Finish listing all current HID devices.\n\n");
    getHidTempPath(0, tempPath);
#endif

    char *typeName = argv[1];
    RF21xDeviceType type = DT_Unknow;
    if (strcmp(typeName, "rf217") == 0)
    {
        type = DT_RF217;
    }
    else if (strcmp(typeName, "rf218") == 0)
    {
        type = DT_RF218;
    }
    else if (strcmp(typeName, "rf219") == 0)
    {
        type = DT_RF219;
    }
    else if (strcmp(typeName, "rf215") ==0)
	{
		type= DT_RF215;
	}
	else
    {
        printf("DeviceType error.\n");
    }

    AbstractRF21x *rf21x = createRF21x(type);
    if (rf21x == NULL)
    {
        printf("Fail to create device handle, please check the DeviceType parameter.");
    }

    std::string sn;
    if (rf21x->readDeviceSerialNumber(argv[2], sn, true) == false)
    {
        printf("fail to readDeviceSerialNumber.\n");
    }
    else
    {
        printf("readDeviceSerialNumber(default): %s \n", sn.c_str());
    }

    if (rf21x->readDeviceSerialNumber(argv[2], sn, false) == false)
    {
        printf("fail to readDeviceSerialNumber.\n");
    }
    else
    {
        printf("readDeviceSerialNumber(user defined): %s \n", sn.c_str());
    }

    sn[0] = '3';
    if (rf21x->writeDeviceSerialNumber(argv[2], sn) == false)
    {
        printf("fail to writeDeviceSerialNumber.\n");
    }
    else
    {
        printf("writeDeviceSerialNumber: %s \n", sn.c_str());
    }

    if (rf21x->readDeviceSerialNumber(argv[2], sn, false) == false)
    {
        printf("fail to readDeviceSerialNumber.\n");
    }
    else
    {
        printf("readDeviceSerialNumber(user defined): %s \n", sn.c_str());
    }



    if (rf21x->open(argv[2], 1, 40))
    {
        printf("Success to open device.\n");
        RF21X_SLEEP(1000);
    }
    else
    {
        printf("Fail to open device.\n");
        return 0;
    }

//    if (rf21x->startDynamicId())
//    {
//        printf("Success to start dynamic ID.\n");
//    }
//    else
//    {
//        printf("Fail to start dynamic ID.\n");
//        return 0;
//    }

//    RF21X_SLEEP(100000);
//    if (rf21x->stopDynamicId())
//    {
//        printf("Success to stop dynamic ID.\n");
//    }
//    else
//    {
//        printf("Fail to stop dynamic ID.\n");
//        return 0;
//    }

    RF21xQuizParams params;
    params.param1 = 2;
    params.param2 = 10;
    if (rf21x->startQuiz(QT_Quiz209,params))//(QT_Single, params))
    {
        printf("Success to start a quiz.\n");
    }
    else
    {
        printf("Fail to start a quiz.\n");
        return 0;
    }

    printf("Getting message from device, press Ctrl+C to break...\n");
    RF21xMessage message;
    while (1)
    {
#ifdef _WIN32
        std::string path = argv[2];
        if (path.size() >= 6 && path.compare(0, 6, "hid://") == 0
                && checkHidCable(tempPath) == false)
        {
            printf("Cable error! \n");
        }
#endif
        if (rf21x->getMessage(message))
        {
            if (message.messageType == MT_Teacher)
            {
                printf("Teacher: %s\n", message.data.c_str());
            }
            else if (message.messageType == MT_Student)
            {
                printf("Student %d for current question: %s\n", message.keypadId, message.data.c_str());
            }
            else if (message.messageType == MT_SetId)
            {
                printf("Set ID %d\n", message.keypadId);
            }

            unsigned char *buf = message.rawDataBuffer;
            int len = message.rawDataLength;
            printf("Raw data: ");
            for (int i = 0; i < len; ++i)
            {
                printf("%02X ", buf[i]);
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
    rf21x->stopQuiz();
    rf21x->close();

    delete rf21x;
    rf21x = NULL;

    return 0;
}
