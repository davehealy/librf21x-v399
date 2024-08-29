#include <stdio.h>
#include <string.h>
#include <vector>

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
    if (argc != 2)
    {
        printf("Name: rf21x-test \n");
        printf("Usage: rf21x-test DeviceType \n");
        printf("  Usage Example: rf21x-test rf217\n");
        printf("  Usage Example: rf21x-test rf218\n");
        printf("  Usage Example: rf21x-test rf219\n");
        printf("Parameters:\n");
        printf("  DeviceType: rf217, rf218, rf219, rf215. \n");
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
    char sn[100];
    char hidPath[256][256];
    int i = 0;
    while (rf21x_getHidSerialNumber(i, sn, sizeof(sn)))
    {
        sprintf(hidPath[i], "hid://%s", sn);
        printf("HID Device %d: %s\n", i, hidPath[i]);
        ++i;
    }
    printf("Finish listing all current HID device.\n\n");

    int total = i;
    if (total < 1)
    {
        printf("Fail to find hid devices.\n");
        return - 1;
    }
    printf("There are %d HID device.\n", total);


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
    else if (strcmp(typeName, "rf215") ==0)
	{
		type = RF21X_DT_RF215;
	}
	else
    {
        printf("DeviceType error.\n");
    }


    rf21x_Device *rf21x = (rf21x_Device*)malloc(total * sizeof(rf21x_Device));

    for (int i = 0; i < total; ++i)
    {
        rf21x[i] = rf21x_createDevice(type);
        if (rf21x[i] == NULL)
        {
            printf("Fail to create device handle, please check the DeviceType parameter.");
        }

        if (rf21x_open(rf21x[i], hidPath[i], 1, 40))
        {
            printf("Success to open HID device %d: %s.\n", i, hidPath[i]);
            RF21X_SLEEP(2000);
        }
        else
        {
            printf("Fail to open HID device %d: %s.\n", i, hidPath[i]);
            continue;
        }

        if (rf21x_startQuiz(rf21x[i], RF21X_QT_Single, 6))
        {
            printf("Success to start a quiz.\n");
        }
        else
        {
            printf("Fail to start a quiz.\n");
        }
    }

    printf("Getting message from device, press Ctrl+C to break...\n");
    rf21x_Message message = rf21x_createMessageBuffer();
    while (1)
    {
        for (int i = 0; i < total; ++i)
        {
            if (rf21x_getMessage(rf21x[i], message))
            {
                printf("Device: %s\n", hidPath[i]);

                int type = rf21x_getMessageType(message);
                int keypadId = rf21x_getKeypadId(message);
                const char* data = rf21x_getData(message);
                if (type == RF21X_MT_Teacher)
                {
                    printf("Teacher: %s\n", data);
                }
                else if (type == RF21X_MT_Student)
                {
                    printf("Student %d for current question: %s\n", keypadId, data);
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
    }
    rf21x_destoryMessageBuffer(message);

    printf("Press any key to stop.\n");
    getchar();

    for (int i = 0; i < total; ++i)
    {
        rf21x_stopQuiz(rf21x[i]);
        rf21x_close(rf21x[i]);
        rf21x_destoryDevice(rf21x[i]);
        printf("Device %d destroyed...\n", i);
    }
    printf("Success Stop quiz and close device.\n");
    free(rf21x);

    return 0;
}
