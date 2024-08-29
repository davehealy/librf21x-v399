/*!
 * \file doxygen.h Main page of document.
 * \mainpage EnjoyARS librf21x SDK API Reference
 *
 * \section intro_sec Introduction
 * EnjoyARS librf21x SDK is a cross platform SDK written in C/C++.
 * EnjoyARS librf21x SDK supply C API for rf21x devices by a dynamic library , and release with a C++ wraps by source code.
 *
 * Supported platforms:
 * Windows XP, Windows Vista, Windows 7, Windows 8. (Serial Port, HID, Socket)
 * Linux with kernel 2.6 or later version. (HID, Socket)
 * Mac. (HID)
 * Android. (Socket) (See "andoird/README.md" for more details)
 *
 * Supported devices type:
 * - rf215
 * - rf217
 * - rf218
 * - rf219
 * - rf228
 *
 * Supperted device interfaces:
 * - serial port
 * - hid serial port
 * - socket
 *
 * \section usage_sec Usage
 * All the API is thread safe. Basic usage:
 * 1. Create device instance: Create a class with a specified device type.
 * 2. Open device: Open device with a specified port URL.
 *     Teacher keypad will be available after this step, user can get message from device instance.
 * 3. Start and stop quiz: Start a quiz.
 *     Student keypad will be available after this step, user can get message from device instance.Then you can stop it.
 * 4. Close device.
 * 5. Destory device instance.
 *
 * Port name example:
 * - serial port device:
 *   - "serial://COM1" (Windows)
 *   - "serial://COM2" (Windows)
 *   - "serial:///dev/ttyUSB0" (Linux)
 *   - "serial:///dev/ttyUSB1" (Linux)
 * - serial port with HID device:
 *   - "hid://"
 *   - "hid://0007800C"
 * - network device:
 *   - "tcp://192.168.1.1:9000+serial://COM1"
 *   - "tcp://192.168.2.2:9000+hid://"
 *
 * Usage example:
 * \code{.cpp}
        #include <stdio.h>
        #include <string.h>

        #include "rf21x-api.h"

        int main(int argc, char **argv)
        {
            rf21x_Device rf21x = rf21x_createDevice(RF21X_DT_RF219);
            rf21x_open(rf21x, "hid://", 1, 40);
            rf21x_startQuiz(rf21x, RF21X_QT_Single, 6);
            rf21x_Message message = rf21x_createMessageBuffer();
            while (1)
            {
                if (rf21x_getMessage(rf21x, message))
                {
                    int type = rf21x_msgType(message);
                    int keypadId = rf21x_msgKeypadId(message);
                    const char* data = rf21x_msgData(message);
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
                }
            }
            rf21x_destoryMessageBuffer(message);
            printf("Press any key to stop.\n");
            printf("Success Stop quiz and close device.\n");
            rf21x_stopQuiz(rf21x);
            rf21x_close(rf21x);
            rf21x_destoryDevice(rf21x);

            return 0;
        }
 * \endcode
 *
 * \section install_sec Install
 * In order to compile librf21x, you need <b>CMake</b>.
 *
 * \section example_sec Example
 * To see the example of librf21x, you can read souce code in example folder.
 */
