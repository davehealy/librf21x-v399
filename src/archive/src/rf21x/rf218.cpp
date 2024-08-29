#include "rf218.h"

#include <stdio.h>
#include <string.h>

void RF218::dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
{
    //printf("event\n");
    static unsigned char buf[GSP_MAX_DATA_SIZE + 1000];
    static int len = 0;
    memcpy(buf + len, data, length);
    len += length;

//    for (int i = 0; i < length; ++i)
//    {
//        printf("123 package data %d: %X\n", i, data[i]);
//    }

    int i = 0;
    int index = 0;
    while (i + 5 - 1 < len)
    {
        if (_type == QT_Homework)
        {
            if (i + 7 - 1 >= len)
            {
                break;
            }
            unsigned char *packageData = buf + i;
            int packageLength = 7;
            if ((packageData[0] & 0x60) != 0x20)
            {
                ++i;
                continue;
            }
            _processPackage(packageData, packageLength);
            i += packageLength;
            index = i;
        }
        else
        {
            unsigned char *packageData = buf + i;
            int packageLength = 5;
            if (_checkPackage(packageData, packageLength) == false)
            {
                ++i;
                continue;
            }
            _processPackage(packageData, packageLength);
            i += packageLength;
            index = i;
        }
    }
    memcpy(buf, buf + index, len - index);
    len = len - index;
}

bool RF218::_checkPackage(unsigned char *data, int retLength)
{
    if (retLength == 5
            && (data[0] & 0x80) == 0
            && data[2] == (data[0] ^ data[1] ^ data[3] ^ data[4]))
    {
        return true;
    }
    return false;
}

void RF218::_processPackage(unsigned char *data, int length)
{
    RF21xMessage message;
    message.quizType = _type;
    message.saveRawData(data, length);
//    for (int i = 0; i < length; ++i)
//    {
//        printf("package data %d: %X\n", i, data[i]);
//    }
    if (_type == QT_Homework)
    {
        message.messageType = MT_Student;
        message.quizNumber = ((data[0] & 0x0F) - 1) * 8 + 1;
        message.keypadId = data[1] & 0x7F;

        std::string answer = "";
        std::string options = "ABCDEF       "; // need space char for empty answer
        answer += options[(data[3] >> 4) & 0x07];
        answer += options[(data[3] >> 0) & 0x07];
        answer += options[(data[4] >> 4) & 0x07];
        answer += options[(data[4] >> 0) & 0x07];
        answer += options[(data[5] >> 4) & 0x07];
        answer += options[(data[5] >> 0) & 0x07];
        answer += options[(data[6] >> 4) & 0x07];
        answer += options[(data[6] >> 0) & 0x07];
        message.data += answer;

        this->_executeMessageEvent(message);

        return;
    }

    message.keypadId = (data[0] & 0x1F) * 0x10 + (data[1] & 0x0F);
    message.quizNumber = data[3] & 0x7F;
    if ((data[0] & 0x60) == 0 && message.keypadId == 0)
    {
        message.messageType = MT_Teacher;

        int key = data[4] & 0x7F;
        if (key >= 0 && key < 12)
        {
            message.data += _teacherMessagesString[key];
        }
    }
    else if ((data[0] & 0x60) == 0 && message.keypadId != 0)
    {
        message.messageType = MT_Student;

        int answerValue = data[4] & 0x7F;
        std::string answer = "";
        std::string options = "ABCDEF";

        // single selection
        if (_type == QT_Single)
        {
            if (answerValue == 7)
            {
                answer = "rush";
            }
            else if (answerValue >= 0 && answerValue < 6)
            {
                answer += options[answerValue];
            }
        }
        // multiple selection
        else if (_type == QT_Multiple)
        {
            if (answerValue == 0)
            {
                answer = "rush";
            }
            else
            {
                for (size_t i = 0; i < options.length(); ++i)
                {
                    if ((answerValue & (1 << i)) != 0)
                    {
                        answer += options[i];
                    }
                }
            }
        }
        // number
        else if (_type == QT_Number || _type == QT_Text)
        {
            if (answerValue == 127)
            {
                answer = "rush";
            }
            else if (answerValue >= 0 && answerValue <= 100)
            {
                char buf[20] = {0};
                sprintf(buf, "%d", answerValue);
                answer = buf;
            }
        }
        message.data = answer;
    }
    // feedback of setting id
    else if ((data[0] & 0x60) == 0x40)
    {
        message.messageType = MT_SetId;
    }
    else if (_type == QT_Unknow)
    {
    }
    else
    {
        return;
    }
    this->_executeMessageEvent(message);
}

RF218::RF218()
{
    _type = QT_Unknow;
}

RF218::~RF218()
{
    close();
}

bool RF218::open(const std::string port, int minId, int maxId)
{
    if (_sp.isOpened() == false)
    {
        close();
    }

    bool ret = 0;

    unsigned char cmdConnect[] = {0x70 | (maxId % 0x10), 0x80 | (maxId / 0x10)};
    unsigned char retConnect[5] = {0};

    if (_sp.open(port, 115200) == false)
    {
        goto failsafe;
    }
    _sp.setRecvEvent(this);

    ret = _sendCommand(cmdConnect, sizeof(cmdConnect),
                       500,
                       retConnect, sizeof(retConnect));
    if (!ret || retConnect[0] != 0x78)
    {
        goto failsafe;
    }
    //printf("cmdConnect\n");

    stopQuiz();

    return true;

failsafe:
    close();
    return false;
}

bool RF218::close()
{
    stopQuiz();
    _sp.close();

    return true;
}

bool RF218::startQuiz(RF21xQuizType type, const RF21xQuizParams &params)
{
    int typeCode = 0;
    int modeCode = 0;

    if (type == QT_Rush || type == QT_Single)
    {
        typeCode = 0;
    }
    else if (type == QT_Multiple)
    {
        typeCode = 1;
    }
    else if (type == QT_Number)
    {
        typeCode = 2;
    }
    else if (type == QT_Homework)
    {
        typeCode = 0x0C;
        int n = params.param1;
        if (n > 100)
        {
            n = 100;
        }
        if (n < 1)
        {
            n = 1;
        }
        if (n % 8 != 0)
        {
            n = n / 8 * 8 + 8;
        }
        unsigned char cmdInit[] = {0x51, 0x80 | n};
        if (_sendCommand(cmdInit, sizeof(cmdInit)))
        {
            _type = type;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if ((type != QT_Homework) & (params.param1 == 1))
    {
        modeCode = 8;
    }

    unsigned char cmdStart[] = {0x5A, 0x80 | typeCode | modeCode};
    if (_sendCommand(cmdStart, sizeof(cmdStart)))
    {
        _type = type;
        return true;
    }
    return false;
}

bool RF218::startQuizDirectly(RF21xQuizType type, void *buffer)
{
	unsigned char *code = (unsigned char*)buffer;
	unsigned char cmdStart[255];
	int i;
	if (type==QT_UserDefine) //自定义发送数据，可以考虑其它型号都增加这一功能
	{
		if (code[0]==0)
		{
			return false;
		}
		else
		{
			int cmdLength=code[0];
			for(i=1;i<=cmdLength;i++)
			{
				cmdStart[i-1]=code[i];
			}
			if (_sp.writeData(cmdStart, cmdLength) != cmdLength )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		unsigned char *code = (unsigned char*)buffer;
		unsigned char cmdStart[] = {0x5A, 0x80 | code[0] | code[1]};
		if (_sendCommand(cmdStart, sizeof(cmdStart)))
		{
			_type = type;
			return true;
		}
		return false;
	}
}

bool RF218::stopQuiz()
{
    unsigned char cmdStart[] = {0x5B, 0x80};
    _sendCommand(cmdStart, sizeof(cmdStart));
    _type = QT_Unknow;

    return true;
}

bool RF218::setKeypadId(int id)
{
    unsigned char cmd[2] = {0x60 | (id % 0x10), 0x80 | (id / 0x10)};
    _sendCommand(cmd, sizeof(cmd));

    return true;
}

unsigned char RF218::GetQuizResult(int id,int QuizNo)
{
	{
		return 0;
	}
}

void RF218::ClearQuizResult()
{
}


bool RF218::checkDeviceSerialNumber(const std::string port, const std::string sn)
{
    if (sn.length() < 8)
    {
        return false;
    }

    if (_sp.isOpened() == false)
    {
        close();
        if (_sp.open(port, 115200) == false)
        {
            return false;
        }
    }

    unsigned char cmd1[2] = {0x55, 0x80};
    _sendCommand(cmd1, sizeof(cmd1));
    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(200));

    unsigned char cmd2[11] = {0x5E, 0x80, 0xDE, 0};
    for (int i = 0; i < 8; ++i)
    {
        cmd2[i + 3] = 0x80 | sn[i];
    }

    unsigned char ret[1];
    _sp.setTimeout(300);
    _sp.disableRecvEvent();
    _sp.flush();
    _sp.writeData(cmd2, sizeof(cmd2));
    int r = _sp.readData(ret, sizeof(ret));
    _sp.enableRecvEvent();
    _sp.setTimeout(50);
    if (!(r == 1 && ret[0] == 122))
    {
        return false;
    }

    return true;
}

bool RF218::_sendCommand(unsigned char *cmd, int cmdLength,
                         int waitTime,
                         unsigned char *ret, int retLength)
{
    if (_sp.isOpened() == false)
    {
        return false;
    }

    bool isOk = true;

    _sp.setTimeout(waitTime);
    //printf("1.disableRecvEvent\n");
    _sp.disableRecvEvent();
    _sp.flush();
    //printf("2.disableRecvEvent\n");
    unsigned char data[100];
    memcpy(data, cmd, cmdLength);
    unsigned char v = 0;
    for (int i = 0; i < cmdLength; ++i)
    {
        v = v ^ cmd[i];
    }
    data[cmdLength] = v;

    if (_sp.writeData(data, cmdLength + 1) != cmdLength + 1)
    {
        isOk = false;
        goto finish;
    }
    //printf("writeData\n");

    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));

    if (ret != NULL)
    {
        if (_sp.readData(ret, retLength) != retLength)
        {
            isOk = false;
            goto finish;
        }
    }
    //printf("readData\n");

finish:
    _sp.enableRecvEvent();
    _sp.setTimeout(50);
    return isOk;
}


