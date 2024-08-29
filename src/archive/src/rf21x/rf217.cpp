#include "rf217.h"

#include <stdio.h>
#include <string.h>

void RF217::dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
{
    //printf("event\n");
    static unsigned char buf[GSP_MAX_DATA_SIZE + 1000];
    static int len = 0;
    memcpy(buf + len, data, length);
    len += length;

    int i = 0;
    int index = 0;
    while (i < len - 2)
    {
        unsigned char *packageData = buf + i;
        int packageLength = 3;
        if (_checkPackage(packageData, packageLength) == false)
        {
            ++i;
            continue;
        }
        _processPackage(packageData, packageLength);
        i += packageLength;
        index = i;
    }
    memcpy(buf, buf + index, len - index);
    len = len - index;
}

bool RF217::_checkPackage(unsigned char *data, int retLength)
{
    if (retLength == 3
            && (data[0] & 0x80) == 0
            && data[2] == (data[0] ^ data[1]))
    {
        return true;
    }
    return false;
}

void RF217::_processPackage(unsigned char *data, int length)
{
    RF21xMessage message;
    message.quizType = _type;
    message.saveRawData(data, length);
//    for (int i = 0; i < length; ++i)
//    {
//        printf("package data %d: %X\n", i, data[i]);
//    }
	if(_Is117)
	{
		if (data[0]==0)
		{
			message.messageType=MT_Teacher;
			message.keypadId=0;
			int kt=data[1] & 0x7f;
			if (kt >= 0 && kt < 12)//10)
			{
				message.data += _teacherMessagesString[kt];
			}
		}
		else if(data[0]<=100)
		{
			message.messageType = MT_Student;
			message.keypadId=data[0];
			int ks=data[1] & 0x7f;
			if (ks >= 0 && ks < 6)
			{
				message.data += ('A' + ks);
			}
			else
			{
				message.data += "rush";
			}
		}
		else if(data[0]==121)
		{
			int kss=data[1] & 0x7f;
			message.messageType = MT_SetId;
			message.keypadId=kss;
		}
		else
		{
			return;
		}
	}
	else
	{
		int c1 = (data[0] & 0x60) >> 5;
		int k1 = data[0] & 0x1F;
		int c2 = (data[1] & 0x70) >> 4;
		int k2 = data[1] & 0x0F;
		message.keypadId = k1 * 0x10 + k2;

		if (c1 == 0 && k1 == 0)
		{
			message.messageType = MT_Teacher;
			message.keypadId = 0;
			int key = data[1] & 0x7F;
			if (key >= 0 && key < 12)
			{
				message.data += _teacherMessagesString[key];
			}
		}
		else if (c1 == 1)
		{
			message.messageType = MT_Student;
			if (c2 >= 0 && c2 < 6)
			{
				message.data += ('A' + c2);
			}
			else
			{
				message.data += "rush";
			}
		}
		else if (c1 == 2 && c2 == 0)
		{
			message.messageType = MT_SetId;
		}
		else
		{
			return;
		}
	}
    this->_executeMessageEvent(message);
}

RF217::RF217()
{
    _type = QT_Unknow;
}

RF217::~RF217()
{
    close();
}

bool RF217::open(const std::string port, int minId, int maxId)
{
    if (_sp.isOpened() == false)
    {
        close();
    }

    bool ret = 0;


	unsigned char cmdConnect117[] = {0x5d, 0x80 | maxId };
	unsigned char retConnect117[3] = {0};

	if (_sp.open(port, 19200) == false)
	{
		goto failsafe_217;
	}
	_sp.setRecvEvent(this);

	ret = _sendCommand(cmdConnect117, sizeof(cmdConnect117),
		300,
		retConnect117, sizeof(retConnect117));
	if (!ret || retConnect117[0] != 0x78)
	{
		goto failsafe_217;
	}
	//printf("cmdConnect 115200\n");
	_Is117=true;
	stopQuiz();

	return true;

failsafe_217:
	_Is117=false;
    unsigned char cmdConnect[] = {0x70 | (maxId % 0x10), 0x80 | (maxId / 0x10)};
    unsigned char retConnect[3] = {0};

    if (_sp.open(port, 115200) == false)
    {
        goto failsafe_28800;
    }
    _sp.setRecvEvent(this);

    ret = _sendCommand(cmdConnect, sizeof(cmdConnect),
                       500,
                       retConnect, sizeof(retConnect));
    if (!ret || retConnect[0] != 0x78)
    {
        goto failsafe_28800;
    }
    //printf("cmdConnect 115200\n");

    stopQuiz();

    return true;

failsafe_28800:
    close();
    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(100));

    if (_sp.open(port, 28800) == false)
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
    //printf("cmdConnect 28800\n");

    stopQuiz();

    return true;

failsafe:
    close();
    return false;
}

bool RF217::close()
{
    stopQuiz();
    _sp.close();

    return true;
}

bool RF217::startQuiz(RF21xQuizType type, const RF21xQuizParams &params)
{
    if (type == QT_Rush || type == QT_Single)
    {
        unsigned char cmdStart[] = {0x5A, 0x80};
        if (_sendCommand(cmdStart, sizeof(cmdStart)))
        {
            _type = type;
            return true;
        }
    }
    return false;
}

bool RF217::startQuizDirectly(RF21xQuizType type, void *buffer)
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
		unsigned char cmdStart[] = {0x5A, 0x80};
		return _sendCommand(cmdStart, sizeof(cmdStart));
	}
}

bool RF217::stopQuiz()
{
    unsigned char cmdStart[] = {0x5B, 0x80};
    _sendCommand(cmdStart, sizeof(cmdStart));
    return true;
}

bool RF217::setKeypadId(int id)
{
	if(_Is117)
	{
		unsigned char cmd117[2] = {0x5c, 0x80 | id };
		_sendCommand(cmd117, sizeof(cmd117));
	}
	else
	{
    unsigned char cmd[2] = {0x60 | (id % 0x10), 0x80 | (id / 0x10)};
    _sendCommand(cmd, sizeof(cmd));
	}
    return true;
}

unsigned char RF217::GetQuizResult(int id,int QuizNo)
{
	{
		return 0;
	}
}

void RF217::ClearQuizResult()
{
}

bool RF217::checkDeviceSerialNumber(const std::string port, const std::string sn)
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
    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(100));


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
    if (r == 1 && ret[0] == 122)
    {
        return true;
    }


    // 28800
    close();
    if (_sp.open(port, 28800) == false)
    {
        return false;
    }

    _sendCommand(cmd1, sizeof(cmd1));
    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(100));

    _sp.setTimeout(300);
    _sp.disableRecvEvent();
    _sp.flush();
    _sp.writeData(cmd2, sizeof(cmd2));
    r = _sp.readData(ret, sizeof(ret));
    _sp.enableRecvEvent();
    _sp.setTimeout(50);
    if (r == 1 && ret[0] == 122)
    {
        return true;
    }

    return false;
}

bool RF217::_sendCommand(unsigned char *cmd, int cmdLength,
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


