/*!
  * \file rf21x-wrap.h
  * Cpp wrap of rf21x device APIs.
  */

#ifndef __RF21X_WRAP_H__
#define __RF21X_WRAP_H__

#include "rf21x-api.h"
#include <string>
#include <vector>

#ifdef SWIG

/*!
 * \name Defination of Device Type
 */
//@{
#define RF21X_DT_Unknow 0 //!< Device Type: Unkonw
#define RF21X_DT_RF217 1 //!< Device Type: RF217
#define RF21X_DT_RF218 2 //!< Device Type: RF218
#define RF21X_DT_RF219 3 //!< Device Type: RF219   ; RF317 use same device type of RF219
#define RF21X_DT_RF215 4 //!< Device Type: RF215
#define RF21X_DT_RF228 5 //!< Device Type: RF228
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
#define RF21X_QT_Text 5 //!< Quiz Type: Text
#define RF21X_QT_Classify 6 //!< Quiz Type: Classify
#define RF21X_QT_Sort 7 //!< Quiz Type: Sort
#define RF21X_Qt_JudgeOrVote 8 //!< Quiz Type: Judge Or Vote
#define RF21X_QT_SelectId 9 //!< Select a buzzer Id (Only for 215)
#define RF21X_QT_Control 10  //!<Control buzzer beep on, beep off and power off (Only for 215)
#define RF21X_QT_SelfPaced 11 //!< Quiz Type: Self_Paced (Only for rf219)
#define RF21X_QT_Notification 12 //!< Quiz Type: Notification (Only for sending auth info to 228)
#define RF21X_QT_Quiz209 13 // !<only for 209
#define RF21X_QT_SendMessage 14// !<only for 209, used in startQuizDirectly
#define RF21X_QT_UserDefine 100 // !<used in startQuizDirectly, first byte is how many bytes will be sent, following with the data which will be sent. 
//@}

#endif

/*!
 * \brief Namespace of rf21x.
 */
namespace rf21x
{

/*!
 * \brief Get HID device serial number.
 *  User can use serial number as address which like "hid://0007800D" to select a hid device to be opened.
 * \return serial number in an vector.
 */
std::vector<std::string> getHidSerialNumber();

#ifdef _WIN32
/*!
 * \brief Get HID device temp path.
 *  Used for checking cable connection.
 * \return temp path in an vector.
 */
std::vector<std::string> getHidTempPath();

/*
 * \brief Check the hardware connection of HID cable.
 * \return Success or fail.
 */
bool checkHidCable(const std::string tempPath);
#endif

/*!
 * \brief The RF21xMessage class
 */
class RF21xMessage
{
public:

    /*!
     * \brief Message type, could be RF21X_MT_Teacher, RF21X_MT_Student, RF21X_MT_SetId.
     */
    int messageType;

    /*!
     * \brief Quiz type, could be RF21X_QT_Rush, RF21X_QT_Single, RF21X_QT_Multiple, etc...
     */
    int quizType;

    /*!
     * \brief Quiz number, only valid in self-pace mode.
     */
    int quizNumber;

    /*!
     * \brief Timestamp, only valid for RF317.
     */
    int timeStamp;

    /*!
     * \brief Keypad ID.
     */
    int keypadId;

    /*!
     * \brief Message content.
     */
    std::string data;

    /*!
     * \brief Raw data for low level usage.
     */
    std::vector<unsigned char> rawData;
};

/*!
 * \brief The RF21xDevice class
 */
class RF21xDevice
{
public:
    /*!
     * \brief RF21xDevice
     */
    explicit RF21xDevice();

    /*!
     * \brief ~RF21xDevice
     */
    virtual ~RF21xDevice();

    /*!
     * \brief Open device with a specified port url.
     * \param type Device type, should be RF21X_DT_RF217, RF21X_DT_RF218, RF21X_DT_RF219, RF21X_DT_RF215.
     * \param port Serial port name.
     * Example of serialPort:
     * - serial port device:
     *   - "serial://COM1" (Windows)
     *   - "serial://COM2" (Windows)
     *   - "serial:///dev/ttyUSB0" (Linux)
     *   - "serial:///dev/ttyUSB1" (Linux)
     * - serial port with HID device:
     *   - "hid://"
     * - network device:
     *   - "tcp://192.168.1.1:9000+serial://COM1"
     *   - "tcp://192.168.2.2:9000+hid://"
     * \param minId Minimal keypad ID of students's keypad. Set it to 1 will be just fine.
     * \param maxId Maximum keypad ID of students's keypad, this will affect speed of keypads' communication.
     * \return Success or fail.
     */
    bool open(int type, const char *port, int minId, int maxId);

    /*!
     * \brief Close device.
     * \return Success or fail.
     */
    bool close();

    /*!
     * \brief Start quiz with the specified quiz type.
     *
     * Quiz Type Support:
     * - rf217: Rush, Single.
     * - rf218: Rush, Single, Multiple, Number, Homework.
     * - rf219: Rush, Single, Multiple, Number, Text, Classify, Sort, Judge or Vote, Homework.
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
     *   - Single: Do not support any parameter
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
	 * \param type Quiz type.
     * \param param1 Parameter 1.
     * \param param2 Parameter 2.
     * \param newQuiz Parameter 3. Only valid for RF219.
     *        newQuiz=0 start same quiz,the input content will be kept;
     *        newQuiz=1 start a new quiz, the input content will be cleared.
     * \return Success or fail.
     */
    bool startQuiz(int type, int param1=-1, int param2=-1, int newQuiz=1);

    /*!
     * \brief Start quiz by raw mode, which ueses hardware manipulating directly.
     * \param type Quiz type, only used for receive message.
     * \param buffer Hardware manipulating codes.
     * \return Success or fail.
     */
    bool startDirectly(int type, void *buffer);

    /*!
     * \brief Stop current quiz.
     * \return Success or fail.
     */
    bool stopQuiz();

    /*!
     * \brief Set keypad ID, which need cooperation with keypad.
     * \param id keypad ID.
     * \return Success or fail.
     */
    bool setKeypadId(int id);

	unsigned char GetQuizResult(int id,int QuizNo);
	void ClearQuizResult();

    /*!
     * \brief Get a 8-bytes length serial number from device. Only worked for some rf219.
     * \param deviceType Same as open function.
     * \param devicePort Same as open function.
     * \param sn A 8-bytes length serial number end with '\0', each byte from 0 to 9. So a number serial is like "01234567\0";
     * \param isDefault Chose to read default serial number or user defined serial number.
     * \return Success or fail.
     */
    virtual bool readDeviceSerialNumber(int deviceType, const char *devicePort, std::string &sn, bool isDefault);

    /*!
     * \brief Set a 8-bytes length serial number from device. Only worked for some rf219.
     * \param deviceType Same as open function.
     * \param devicePort Same as open function.
     * \param sn A 8-bytes serial number, each byte from 0 to 9. So a serial is like "01234567";
     * \return Success or fail.
     */
    virtual bool writeDeviceSerialNumber(int deviceType, const char *devicePort, const std::string sn);

    /*!
     * \brief Reset to default serial number from device. Only worked for some rf219.
     * \param deviceType Same as open function.
     * \param devicePort Same as open function.
     * \return Success or fail.
     */
    virtual bool resetDeviceSerialNumber(int deviceType, const char *devicePort);

    /*!
     * \brief Check a 8-bytes length serial number from device. Worked for all devices.
     * \param deviceType Same as open function.
     * \param devicePort Same as open function.
     * \param sn A 8-bytes serial number, each byte from 0 to 9. So a serial is like "01234567";
     * \return Success or fail.
     */
    virtual bool checkDeviceSerialNumber(int deviceType, const char *devicePort, const std::string sn);

    /*!
     * \brief Try to get message from device.
     * \param message Input and output class.
     * \return Success or fail.
     */
    bool getMessage(RF21xMessage &message);

private:
    rf21x_Device _device;
    rf21x_Message _message;
};

}

#endif
