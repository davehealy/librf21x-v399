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
    // User can use serial number as address like "hid://0007800D" to select a hid device to be opened.
    printf("Start listing all current HID device:\n");
    char buf[100];
    int i = 0;
    while (rf21x_getHidSerialNumber(i, buf, sizeof(buf)))
    {
        printf("HID Device %d: %s \n", i, buf);
        ++i;
    }
    printf("Finish listing all current HID device. \n\n");

    rf21x_Device rf21x = rf21x_createDevice(RF21X_DT_RF228);
    if (rf21x == NULL)
    {
        printf("Fail to create device handle, please check the DeviceType parameter.");
    }

    if (rf21x_open(rf21x, "hid://", 1, 40))
    {
        printf("Success to open device.\n");
        RF21X_SLEEP(2000);
    }
    else
    {
        printf("Fail to open device.\n");
        return 0;
    }

//    rf21x_setKeypadId(rf21x, 0x1a3b5c);
//    return 0;

//    if (rf21x_startQuiz(rf21x, RF21X_QT_Number, 10))
//    {
//        printf("Success to start a quiz.\n");
//    }
//    else
//    {
//        printf("Fail to start a quiz.\n");
//        return 0;
//    }

    printf("Getting message from device, press Ctrl+C to break... \n");
    printf("Press Fn+2 on keypad to start auth mode, the password is 123456 \n");
    rf21x_Message message = rf21x_createMessageBuffer();
    while (1)
    {
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
                    printf("Answer from ID %d for current question: %s \n", keypadId, data);
                }
                else
                {
                    printf("Answer from ID %d for question %d: %s \n", keypadId, quizNumber, data);
                }
            }
            else if (type == RF21X_MT_Auth)
            {
                printf("Auth from keypad SN 0x%X: user=%d, pwd=%s \n", keypadId, quizNumber, data);
                if (strcmp("123456", data) == 0)
                {
                    printf("Auth success \n");
                    rf21x_startQuiz(rf21x, RF21X_QT_Notification, keypadId, 1);
                }
                else
                {
                    printf("Auth fail \n");
                    rf21x_startQuiz(rf21x, RF21X_QT_Notification, keypadId, 0);
                }
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
