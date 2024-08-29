/*!
  * \file rf21x-api.h
  * C APIs of rf21x device.
  */

#ifndef __RF21X_API_H__
#define __RF21X_API_H__


/*!
 * \def LIBRF21X_API
 * Internal usage.
 */

/*!
 * \def API_FUNC
 * Internal usage.
 */

// for SWIG
#ifdef _MSC_VER
    #define API_FUNC __stdcall
    #ifdef LIBRF21X_EXPORTS
        #define LIBRF21X_API __declspec(dllexport)
    #else
        #define LIBRF21X_API __declspec(dllimport)
    #endif
#else
    #define API_FUNC
    #define LIBRF21X_API
#endif

/*!
 * \name Defination of Device Type
 */
//@{
#define RF21X_DT_Unknow 0 //!< Device Type: Unkonw
#define RF21X_DT_RF217 1 //!< Device Type: RF217
#define RF21X_DT_RF218 2 //!< Device Type: RF218
#define RF21X_DT_RF219 3 //!< Device Type: RF219; RF317(only support Single quiz type,get message with timestamp) 
#define RF21X_DT_RF215 4 //!< Device Type: RF215
#define RF21X_DT_RF228 5 //!< Device Type: RF215
//@}


/*!
 * \name Defination of Message Type
 */
//@{
#define RF21X_MT_Unknow 0 //!< Message Type: Unkonw
#define RF21X_MT_Teacher 1 //!< Message Type: Teacher
#define RF21X_MT_Student 2 //!< Message Type: Student
#define RF21X_MT_SetId 3 //!< Message Type: SetId
#define RF21X_MT_Auth 4 //!< Message Type: Auth
#define RF21X_MT_RecSn 5//!<only for 209
#define RF21X_MT_RecVersion 6//!<only for 209
#define RF21X_MT_RecString 7//!<only for 209
#define RF21X_MT_RecChannel 8//!<only for 209
#define RF21X_MT_RecPower 9	//!<only for 209
#define RF21X_MT_AllQuResult 10 //only for 209  ; 返回指定ID的所有题号的结果记录
#define RF21X_MT_AllIdResult 11 //only for 209 ;返回指定题号的所有ID的结果记录

//@}


/*!
 * \name Defination of Quiz Type
 */
//@{
#define RF21X_QT_Homewrok -1 //!< Quiz Type: Homework
#define RF21X_QT_Unknow 0 //!< Quiz Type: Unkonw
#define RF21X_QT_Rush 1 //!< Quiz Type: Rush
#define RF21X_QT_Single 2 //!< Quiz Type: Single
#define RF21X_QT_Multiple 3 //!< Quiz Type: Multiple
#define RF21X_QT_Number 4 //!< Quiz Type: Number 
#define RF21X_QT_Text 5 //!< Quiz Type: Text (Only for 219)
#define RF21X_QT_Classify 6 //!< Quiz Type: Classify (Only for 219)
#define RF21X_QT_Sort 7 //!< Quiz Type: Sort (Only for 219)
#define RF21X_Qt_JudgeOrVote 8 //!< Quiz Type: Judge Or Vote (Only for 219)
#define RF21X_QT_SelectId 9 //!< Select a buzzer Id (Only for 215)
#define RF21X_QT_Control 10  //!<Control buzzer beep on, beep off and power off (Only for 215)
#define RF21X_QT_SelfPaced 11 //!< Quiz Type: Self_Paced (Only for rf219)
#define RF21X_QT_Notification 12 //!< Quiz Type: Notification (Only for sending auth info to 228)
#define RF21X_QT_Quiz209 13 // !<only for 209
#define RF21X_QT_SendMessage 14// !<only for 209, used in startQuizDirectly
#define RF21X_QT_UserDefine 100 // !<used in startQuizDirectly, first byte is how many bytes will be sent, following with the data which will be sent. 
//@}

/*!
 * \brief Device handle of rf21x.
 */
typedef void* rf21x_Device;

/*!
 * \brief Message buffer handle of rf21x.
 */
typedef void* rf21x_Message;


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \name Device
 */
//@{

/*!
 * \brief Create a device instance with a specified device type.
 * \param deviceType Device type, RF21X_DT_xxx.
 */
LIBRF21X_API rf21x_Device API_FUNC rf21x_createDevice(int deviceType);

/*!
 * \brief Destory a device instance.
 */
LIBRF21X_API void API_FUNC rf21x_destoryDevice(rf21x_Device device);

/*!
 * \brief Get HID device serial number.
 *  User can use serial number as address whick like "hid://0007800D" to select a hid device to be opened.
 * \param index Index number of device, start from 0.
 * \param buffer Serial number got from device.
 * \param maxLength max length.
 * \return success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_getHidSerialNumber(int index, char *buffer, int maxLength);

#ifdef _WIN32
/*!
 * \brief Get HID device temp path.
 *  User can use temp path to check cable.
 * \param index Index number of device, start from 0.
 * \param buffer Temp path got from device.
 * \param maxLength max length.
 * \return success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_getHidTempPath(int index, char *buffer, int maxLength);

/*!
 * \brief Check the hardware connection of HID cable.
 * \param tempPath Temp path of device.
 * \return success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_checkHidCable(const char *tempPath);
#endif

/*!
 * \brief Open device with a specified port url.
 * \param device Device handle.
 * \param port Serial port name.
 * Example of serialPort:
 * - serial port device:
 *   - "serial://COM1" (Windows)
 *   - "serial://COM2" (Windows)
 *   - "serial:///dev/ttyUSB0" (Linux)
 *   - "serial:///dev/ttyUSB1" (Linux)
 * - serial port with HID device:
 *   - "hid://"
 *   - "hid://0007800D"
 * - network device:
 *   - "tcp://192.168.1.1:9000+serial://COM1"
 *   - "tcp://192.168.2.2:9000+hid://"
 * \param minId Minimal keypad ID of students's keypad. Set it to 1 will be just fine.
 * \param maxId Maximum keypad ID of students's keypad, this will affect speed of keypads' communication.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_open(rf21x_Device device, const char *port, int minId, int maxId);

/*!
 * \brief Close device.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_close(rf21x_Device device);

/*!
 * \brief Parameters of different quiz type and device type.
 *
 * Quiz Type Support:
 * - rf217: Rush, Single.
 * - rf218: Rush, Single, Multiple, Number, Homework.
 * - rf219: Rush, Single, Multiple, Number, Text, Classify, Sort, Judge or Vote,SelfPaced, Homework.
 * - rf215: Rush, Single, SelectId, Control.
 * - rf228: Single, Multiple, Number, Notification.
 *
 * Parameters Support:
 * - rf217: Do not support any parameter.
 * - rf218:
 *   - Single, Multiple, Number:
 *     - param1:
 *       - 0 = one-by-one mode;
 *       - 1 = self-pace mode.
 *   - Homework:
 *     - param1: number of questions, 1~100
 *     - usage of rf218:
 *       - Only support single type, limited by number of 100, result returned by question groups, each group has 8 questions.
 *       - Fn+9: Enter homework mode.
 *       - Fn+0: Exit homework mode.
 *       - '<' & '>': Switch question.
 *       - Fn+>: Goto question.
 *       - Fn+CE: Clear all.
 *       - Fn+OK: Send all.
 * - rf219: Support different quiz type.
 *   - Single:
 *     - param1: Number of total options, 1~10.
 *   - Multiple:
 *     - param1: Number of total options, 1~8.
 *     - param2: Number of right options, 1~4 for 1~4 options, 5~8 for any options.
 *   - Text:
 *     - param1: Max length of message.
 *   - Classify:
 *     - param1: Number of total options, 1~10.
 *     - param2: Number of total classes, 2~6.
 *   - Sort:
 *     - param1: Number of total options, 1~9.
 *   - Judge or Vote:
 *     - param1: Number of total options, 1~3.
 *     - param2: Display type of judge or vote quiz:
 *       - 10 = Right / Wrong / None (Image)
 *       - 11 = 赞成 / 反对 / 弃权 (Chinese)
 *       - 12 = True / False / Don't know
 *       - 13 = Yes / No / Unsuer
 *       - 14 = votes for / against / abstention
 *       - 15 = Positive / negative / neutral
 *   - SelfPaced:
 *     - param1: number of questions 1~100, only for rf219.
 *   - Homework:
 *     - param1: number of questions, 1~100.
 *     - param2: subject, from 1~7, rf219 only.
 *     - usage of rf219:
 *       - Only support text type, limited by number of 100.
 *       - Fn+4: Enter homework mode.
 *       - Fn+4: Exit homework mode.
 *       - '<' & '>': Switch question.
 *       - Fn+1: Goto question.
 *       - Fn+CE: Clear all.
 *       - Fn+OK: Send all.
 * - rf215:
 *   - Rush: (The fastest one will flash and beep)
 *     - param1:
 *       - 1 = Start new rush quiz;
 *       - 2 = Continue last rush quiz;
 *	   - param2:
 *       - 1 = Only Rush button;
 *       - 2 = Rush button with option A,B,C and D select;
 *       - 3 = Only option A,B,C and D select.
 *   - Single:Do not support any parameter
 *   - Setlect Id : (The select buzzer will flash and beep)
 *     - param1: The buzzer Id value, 1~100;
 *     - param2:
 *       - 1 = The buzzer only flash, no need reply;
 *       - 2 = The buzzer need to select one option from A,B,C or D,
 *   - Control
 *     - param1:
 *       - 1 = Beep ON;
 *       - 2 = Beep OFF;
 *       - 3 = Power OFF buzzer.
 * - rf228:
 *   - Single, Multiple, Number:
 *     - param1:
 *       - 0 = one-by-one mode;
 *       - 1~100 = self-pace mode with sepecific number of questions.
 *   - Notification:
 *     - param1: SN of keypad.
 *     - param2:
 *       - 1 = Success;
 *       - 0 = Fail.
 *     - usage of auth：
 *       1. Press "Fn+2" to enter auth mode, enter user id and pwd to auth;
 *       2. The auth request will be returned in a message of type Auth.
 *       3. The message consists of user id in quizNumber, pwd in data, and SN in keypadID.
 *
 * \param device Device handle
 * \param type Quiz type.
 * \param param1 Parameter 1.
 * \param param2 Parameter 2.
 * \param newQuiz Parameter 3. Only valid for RF219.
 *        newQuiz=0 start same quiz,the input content will be kept;
 *        newQuiz=1 start a new quiz, the input content will be cleared.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_startQuiz(rf21x_Device device, int type, int param1=-1, int param2=-1, int newQuiz=1);

/*!
 * \brief Start quiz by raw mode, which ueses hardware manipulating directly.
 * \param device Device handle.
 * \param type Quiz type, only used for receive message.
 * \param buffer Hardware manipulating codes.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_startQuizDirectly(rf21x_Device device, int type, void *buffer);

/*!
 * \brief Stop current quiz.
 * \param device Device handle.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_stopQuiz(rf21x_Device device);

/*!
 * \brief Set keypad ID, which need cooperation with keypad.
 * \param device Device handle.
 * \param id keypad ID.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_setKeypadId(rf21x_Device device, int id);

/*!
 * \brief Get a 8-bytes length serial number from device. Only worked for some rf219.
 * \param device Device handle.
 * \param port Same as open function.
 * \param sn A 8-bytes length serial number end with '\0', each byte from 0 to 9. So a serial number is like "01234567\0";
 * \param length Length of sn buffer, must be bigger then 8.
 * \param isDefault Chose to read default serial number or user defined serial number.
 * \return Success or fail.
 */
LIBRF21X_API int API_FUNC rf21x_readDeviceSerialNumber(rf21x_Device device, const char *port, char *sn, int length, int isDefault);

/*!
 * \brief Set a 8-bytes length serial number from device. Only worked for some rf219.
 * \param device Device handle.
 * \param port Same as open function.
 * \param sn A 8-bytes length serial number end with '\0', each byte from 0 to 9. So a serial number is like "01234567\0";
 * \return Success or fail.
 */
LIBRF21X_API int API_FUNC rf21x_writeDeviceSerialNumber(rf21x_Device device, const char *port, const char *sn);

/*!
 * \brief Reset to default serial number from device. Only worked for some rf219.
 * \param device Device handle.
 * \param port Same as open function.
 * \return Success or fail.
 */
LIBRF21X_API int API_FUNC rf21x_resetDeviceSerialNumber(rf21x_Device device, const char *port);

/*!
 * \brief Check a 8-bytes length serial number from device. Worked for all devices.
 * \param device Device handle.
 * \param port Same as open function.
 * \param sn A 8-bytes serial number, each byte from 0 to 9. So a serial is like "01234567";
 * \return Success or fail.
 */
LIBRF21X_API int API_FUNC rf21x_checkDeviceSerialNumber(rf21x_Device device, const char *port, const char *sn);

//@}


/*!
 * \name Message
 */
//@{

/*!
 * \brief Create a message buffer.
 */
LIBRF21X_API rf21x_Message API_FUNC rf21x_createMessageBuffer();

/*!
 * \brief Destory a message buffer.
 */
LIBRF21X_API void API_FUNC rf21x_destoryMessageBuffer(rf21x_Message message);

/*!
 * \brief Get message from device, this operation will fill message buffer is success.
 * \param device Device handle.
 * \param message Message buffer handle.
 * \return Success or fail(0).
 */
LIBRF21X_API int API_FUNC rf21x_getMessage(rf21x_Device device, rf21x_Message message);

/*!
 * \brief Return message type of message buffer.
 */
LIBRF21X_API int API_FUNC rf21x_getMessageType(const rf21x_Message message);

/*!
 * \brief Return quiz type of message buffer.
 */
LIBRF21X_API int API_FUNC rf21x_getQuizType(const rf21x_Message message);

/*!
 * \brief Return quiz number of message buffer. Quiz number only valid in self-pace mode.
 */
LIBRF21X_API int API_FUNC rf21x_getQuizNumber(const rf21x_Message message);

/*!
 * \brief Return quiz Timestamp of message buffer. Timestamp only valid RF317.
 */
LIBRF21X_API int API_FUNC rf21x_getTimeStamp(const rf21x_Message message);

/*!
 * \brief Return keypad ID of message buffer.
 */
LIBRF21X_API int API_FUNC rf21x_getKeypadId(const rf21x_Message message);

/*!
 * \brief Return message data of message buffer.
 */
LIBRF21X_API const char* API_FUNC rf21x_getData(const rf21x_Message message);

/*!
 * \brief Get message raw data from message buffer.
 */
LIBRF21X_API void API_FUNC rf21x_getRawData(const rf21x_Message message, unsigned char **dataPtr, int *dataLength);

//@}
LIBRF21X_API unsigned char API_FUNC rf21x_GetQuizResult(rf21x_Device device,int id,int QuizNo);
LIBRF21X_API void API_FUNC	rf21x_ClearQuizResult(rf21x_Device device);


#ifdef __cplusplus
}
#endif

#endif
