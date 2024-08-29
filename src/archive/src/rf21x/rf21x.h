/*!
  * \file rf21x.h
  * C++ APIs of rf21x device.
  */

#ifndef __RF21X_H__
#define __RF21X_H__

#include <string>
#include <vector>

// max raw data is rf219's text message, which is about 130 length, so 200 is enough and compact.
#define RF21X_MAX_RAW_DATA_LENGTH 200


/*!
  * \brief Device type.
  */
enum RF21xDeviceType
{
    DT_Unknow = 0,
    DT_RF217 = 1,
    DT_RF218 = 2,
    DT_RF219 = 3,
    DT_RF215 = 4,
    DT_RF228 = 5
};


/*!
  * \brief Message type.
  */
enum RF21xMessageType
{
    MT_Unknow = 0,
    MT_Teacher = 1,
    MT_Student = 2,
    MT_SetId = 3,
    MT_Auth = 4,
	MT_RecSn=5,      //only for 209
	MT_RecVersion=6,//only for 209
	MT_RecString=7,//only for 209
	MT_RecChannel=8,//only for 209
	MT_RecPower=9,	//only for 209
	MT_AllQuResult=10, //only for 209  ; 返回指定ID的所有题号的结果记录
	MT_AllIdResult=11  //only for 209 ;返回指定题号的所有ID的结果记录
};


/*!
  * \brief Quiz type.
  */
enum RF21xQuizType
{
    QT_Homework = -1, // only for 218, 219
    QT_Unknow = 0,
    QT_Rush = 1,
    QT_Single = 2,
    QT_Multiple = 3,  //only for 218, 219
    QT_Number = 4,   // only for 219
    QT_Text = 5,   // only for 219
    QT_Classify = 6, // only for 219
    QT_Sort = 7,   // only for 219
    Qt_JudgeOrVote = 8,  // only for 219
    QT_SelectId = 9, // only for 215
    QT_Control = 10 , // only for 215, 3 controls: beep on, beep off, buzzer power off
	// and for 209 used in startQuizDirectly,first byte:1=set/2=read channel, 3=set/4=read power,5=power off all, 6=write/7=read receiver name, 8=read receiver sn,
	// 9=read receiver version, 10set can sleep/11=set can power off, 12set can not sleep/13=set can not power off
    QT_SelfPaced = 11, // only for 219
    QT_Notification = 12, // only for sending auth info to 228
	QT_Quiz209=13, // only for 209
	QT_SendMessage=14,// only for 209, used in startQuizDirectly
	QT_UserDefine=100 // used in startQuizDirectly, first byte is how many bytes will be sent, following with the data which will be sent. 
};


/*!
  * \brief Message class of device's message.
  */
class RF21xMessage
{
public:
    /*!
     * \brief Constructor function.
     */
    explicit RF21xMessage();

    /*!
     * \brief Set raw data.
     *
     * Implement by memcpy function. Data saved should access by public varibles directly.
     */
    void saveRawData(unsigned char *buffer, int length);

public:
    /*!
     * \brief Message type.
     */
    RF21xMessageType messageType;

    /*!
     * \brief Quiz type.
     */
    RF21xQuizType quizType;

    /*!
     * \brief Keypad ID.
     */
    int keypadId;

    /*!
     * \brief Quiz number, only valid in self-pace mode.
     */
    int quizNumber;
	int TimeStamp;

    /*!
     * \brief Message data.
     */
    std::string data;
	
public:
    /*!
     * \brief Raw message data buffer, used for raw mode.
     */
    unsigned char rawDataBuffer[RF21X_MAX_RAW_DATA_LENGTH];

    /*!
     * \brief Raw message data length, used for raw mode.
     */
    int rawDataLength;
};


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
 */
class RF21xQuizParams
{
public:
    /*!
     * Constructor function.
     */
    RF21xQuizParams();

public:
    /*!
     * Parameter 1.
     */
    int param1;

    /*!
     * Parameter 2.
     */
    int param2;

	int newQuiz;
};


/*!
  * \brief Abstract API class of rf21x. All the API is thread safe.
  */
class AbstractRF21x
{
public:
    explicit AbstractRF21x();
    virtual ~AbstractRF21x();

public:
    /*!
     * \brief Open device with a specified port url.
     * \param port Serial port name.
     * Example of serialPort:
     * - serial port device:
     *   - "serial://COM1" (Windows)
     *   - "serial://COM2" (Windows)
     *   - "serial:///dev/ttyUSB0" (Linux)
     *   - "serial:///dev/ttyUSB1" (Linux)
     * - serial port with HID device:
     *   - "hid://"
     *   - "hid://0007800C"
     *   - "hid://0007800D"
     * - network device:
     *   - "tcp://192.168.1.1:9000+serial://COM1"
     *   - "tcp://192.168.2.2:9000+hid://"
     * \param minId Minimal keypad ID of students's keypad. Set it to 1 will be just fine.
     * \param maxId Maximum keypad ID of students's keypad, this will affect speed of keypads' communication.
     * \return Success or fail.
     */
    virtual bool open(const std::string port, int minId, int maxId) = 0;

    /*!
     * \brief Close device.
     * \return Success or fail.
     */
    virtual bool close() = 0;

    /*!
     * \brief Start quiz with the specified quiz type.
     * \param type Quiz type.
     * \param params Parameters of different quiz type and device type.
     * \return Success or fail.
     */
    virtual bool startQuiz(RF21xQuizType type, const RF21xQuizParams &params) = 0;

    /*!
     * \brief Start quiz by raw mode, which ueses hardware manipulating directly.
     * \param type Quiz type, only used for receive message.
     * \param buffer Hardware manipulating codes.
     * \return Success or fail.
     */
    virtual bool startQuizDirectly(RF21xQuizType type, void *buffer) = 0;

    /*!
     * \brief Stop current quiz.
     * \return Success or fail.
     */
    virtual bool stopQuiz() = 0;

    /*!
     * \brief Set keypad ID, which need cooperation with keypad.
     * \return Success or fail.
     */
    virtual bool setKeypadId(int id) = 0;

    /*!
     * \brief Get message of device with no blocking. All the API is thread safe.
     * You can get message periodicly in a timer or a child thread.
     * \return Success or fail.
     */
    virtual bool getMessage(RF21xMessage &message) = 0;

    /*!
     * \brief Get a 8-bytes length serial number from device. Only worked for some rf219.
     * \param port Same as open function.
     * \param sn A 8-bytes length serial number end with '\0', each byte from 0 to 9. So a number serial is like "01234567\0";
     * \param isDefault Chose to read default serial number or user defined serial number.
     * \return Success or fail.
     */
    virtual bool readDeviceSerialNumber(const std::string port, std::string &sn, bool isDefault) { return false; }

    /*!
     * \brief Set a 8-bytes length serial number from device. Only worked for some rf219.
     * \param port Same as open function.
     * \param sn A 8-bytes serial number, each byte from 0 to 9. So a serial is like "01234567";
     * \return Success or fail.
     */
    virtual bool writeDeviceSerialNumber(const std::string port, const std::string sn) { return false; }

    /*!
     * \brief Reset to default serial number from device. Only worked for some rf219.
     * \param port Same as open function.
     * \return Success or fail.
     */
    virtual bool resetDeviceSerialNumber(const std::string port) { return false; }

    /*!
     * \brief Check a 8-bytes length serial number from device. Worked for all devices.
     * \param port Same as open function.
     * \param sn A 8-bytes serial number, each byte from 0 to 9. So a serial is like "01234567";
     * \return Success or fail.
     */
    virtual bool checkDeviceSerialNumber(const std::string port, const std::string sn) { return false; }

    virtual bool startDynamicId() { return false; }
    virtual bool stopDynamicId() { return false; }
    virtual bool doDynamicId(int id) { return false; }

	virtual unsigned char GetQuizResult(int id,int QuizNo)=0;
	virtual void ClearQuizResult()=0;

};


/*!
 * \brief Create a device instance, User should destory this instance when it is useless.
 * \param type Device type.
 * \return Device class pointer. Return NULL if fail.
 */
AbstractRF21x* createRF21x(RF21xDeviceType type);

/*!
 * \brief Destroy a device instance.
 * \param rf21x device instance.
 */
void destoryRF21x(AbstractRF21x* rf21x);

/*!
 * \brief Get HID device serial number.
 *  User can use serial number as address whick like "hid://0007800D" to select a hid device to be opened.
 * \param index Index number of device, start from 0.
 * \param str Serial number got from device.
 * \return success or fail.
 */
bool getHidSerialNumber(int index, std::string &str);

#ifdef _WIN32
bool getHidTempPath(int index, std::string &str);
bool checkHidCable(const std::string str);
#endif

#endif
