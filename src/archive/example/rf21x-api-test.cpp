#include <stdio.h>
#include <string.h>
#include <string>

#include "rf21x-api.h"

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
        printf("  Usage Example: rf21x-api-test rf217 hid:// \n");
        printf("  Usage Example: rf21x-api-test rf217 hid://0007800D \n");
        printf("  Usage Example: rf21x-api-test rf217 serial:///dev/ttyUSB0 \n");
        printf("  Usage Example: rf21x-api-test rf217 tcp://192.168.1.10:9000+hid:// \n");
        printf("Parameters:\n");
        printf("  DeviceType: rf217, rf218, rf219, rf215, rf228. \n");
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
    char buf[100];
    int i = 0;
    while (rf21x_getHidSerialNumber(i, buf, sizeof(buf)))
    {
        printf("HID Device %d: %s\n", i, buf);
        ++i;
    }
    printf("Finish listing all current HID device.\n\n");

#ifdef _WIN32
    char tempPath[256];
    rf21x_getHidTempPath(0, tempPath, sizeof(tempPath));
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
    else if (strcmp(typeName, "rf228") == 0)
    {
        type = RF21X_DT_RF228;
    }
	else
    {
        printf("DeviceType error.\n");
    }


    rf21x_Device rf21x = rf21x_createDevice(type);
    if (rf21x == NULL)
    {
        printf("Fail to create device handle, please check the DeviceType parameter.");
    }

//    if (rf21x_resetDeviceSerialNumber(rf21x, argv[2]) == 0)
//    {
//        printf("fail to resetDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("resetDeviceSerialNumber.\n");
//    }

//    char sn[10];
//    if (rf21x_readDeviceSerialNumber(rf21x, argv[2], sn, sizeof(sn), 1) == 0)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(default): %s \n", sn);
//    }

//    if (rf21x_readDeviceSerialNumber(rf21x, argv[2], sn, sizeof(sn), 0) == 0)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(user defined): %s \n", sn);
//    }

////    sn[0] = '0';
//    if (rf21x_writeDeviceSerialNumber(rf21x, argv[2], sn) == false)
//    {
//        printf("fail to writeDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("writeDeviceSerialNumber: %s \n", sn);
//    }

//    if (rf21x_readDeviceSerialNumber(rf21x, argv[2], sn, sizeof(sn), false) == 0)
//    {
//        printf("fail to readDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("readDeviceSerialNumber(user defined): %s \n", sn);
//    }

//    if (rf21x_checkDeviceSerialNumber(rf21x, argv[2], sn) == 0)
//    {
//        printf("Fail to checkDeviceSerialNumber.\n");
//    }
//    else
//    {
//        printf("Success to checkDeviceSerialNumber.\n");
//    }

    RF21X_SLEEP(1000);

    if (rf21x_open(rf21x, argv[2], 1, 40))
    {
        printf("Success to open device.\n");
        RF21X_SLEEP(2000);
    }
    else
    {
        printf("Fail to open device.\n");
        return 0;
    }

    if (rf21x_startQuiz(rf21x, RF21X_QT_Single))
    {
        printf("Success to start a quiz.\n");
    }
    else
    {
        printf("Fail to start a quiz.\n");
        return 0;
    }

    printf("Getting message from device, press Ctrl+C to break...\n");
    rf21x_Message message = rf21x_createMessageBuffer();
    while (1)
    {
#ifdef _WIN32
        std::string path = argv[2];
        if (path.size() >= 6 && path.compare(0, 6, "hid://") == 0
                && rf21x_checkHidCable(tempPath) == 0)
        {
            printf("HID cable error! \n");
        }
#endif
        if (rf21x_getMessage(rf21x, message))
        {
            int type = rf21x_getMessageType(message);
            int keypadId = rf21x_getKeypadId(message);
            int quizNumber = rf21x_getQuizNumber(message);
            const char* data = rf21x_getData(message);
            if (type == RF21X_MT_Teacher)
            {
                printf("Teacher: %s\n", data);
            }
            else if (type == RF21X_MT_Student)
            {
                if (quizNumber <= 0)
                {
                    printf("Student %d for current question: %s\n", keypadId, data);
                }
                else
                {
                    printf("Student %d for question %d: %s\n", keypadId, quizNumber, data);
                }
            }
            else if (type == RF21X_MT_SetId)
            {
                printf("Set ID %d\n", keypadId);
            }

            unsigned char *buf = NULL;
            int len = 0;
            rf21x_getRawData(message, &buf, &len);
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
    rf21x_destoryMessageBuffer(message);

    printf("Press any key to stop.\n");
    getchar();
    printf("Success Stop quiz and close device.\n");
    rf21x_stopQuiz(rf21x);
    rf21x_close(rf21x);
    rf21x_destoryDevice(rf21x);

    return 0;
}
