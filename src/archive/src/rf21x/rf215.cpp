#include "rf215.h"

#include <stdio.h>
#include <string.h>

void RF215::dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
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

bool RF215::_checkPackage(unsigned char *data, int retLength)
{
    if (retLength == 3
            && (data[0] & 0x80) == 0
            && data[2] == (data[0] ^ data[1]))
    {
        return true;
    }
    return false;
}

void RF215::_processPackage(unsigned char *data, int length)
{
    RF21xMessage message;
    message.quizType = _type;
    message.saveRawData(data, length);
//    for (int i = 0; i < length; ++i)
//    {
//        printf("package data %d: %X\n", i, data[i]);
//    }

    int c1 = (data[0] & 0x60) >> 5;
    int k1 = data[0] & 0x1F;
    int c2 = (data[1] & 0x70) >> 4;
    int k2 = data[1] & 0x0F;
    message.keypadId = data[1] & 0x0f;

    if (c1 == 0)
	{
		if ( k1 == 0)
		{
			message.messageType = MT_Teacher;
			message.keypadId = 0;
			int key =k2;// data[1] & 0x7F;
			if (key >= 0 && key < 12)
			{
				message.data += _teacherMessagesString[key];
			}
		}
		else if (k1==1)
		{
			message.messageType=MT_Student;
			message.data+="rush";
		}
		else if (k1==2)
		{
			message.messageType=MT_Student;
			message.data+="A";
		}
		else if (k1==3)
		{
			message.messageType=MT_Student;
			message.data+="B";
		}
		else if (k1==4)
		{
			message.messageType=MT_Student;
			message.data+="C";
		}
		else if (k1==5)
		{
			message.messageType=MT_Student;
			message.data+="D";
		}
	}
	else if (c1==1)
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
		message.keypadId= k1 * 0x10 + k2;
    }
    else
    {
        return;
    }
    this->_executeMessageEvent(message);
}

RF215::RF215()
{
    _type = QT_Unknow;
}

RF215::~RF215()
{
    close();
}

bool RF215::open(const std::string port, int minId, int maxId)
{
    close();

    bool ret = 0;

    unsigned char cmdConnect[] = {0x01, 0x80 | (maxId & 0x7f)};
    unsigned char retConnect[3] = {0};

    if (_sp.open(port, 115200) == false)
    {
        goto failsafe;
    }
    _sp.setRecvEvent(this);

    ret = _sendCommand(cmdConnect, sizeof(cmdConnect),
                       500,
                       retConnect, sizeof(retConnect));
    if (!ret || retConnect[0] != 0x7e)
    {
        goto failsafe;
    }
    //printf("cmdConnect 115200\n");

    stopQuiz();

    return true;

failsafe:
    close();
    return false;
}

bool RF215::close()
{
    stopQuiz();
    _sp.close();

    return true;
}

bool RF215::startQuiz(RF21xQuizType type, const RF21xQuizParams &params)
{
    int typeCode = 0;
    int paramCode = 0;
	if (type==QT_Rush)
	{
		if (params.param1==2)
		{
			if(params.param2==3)
			{
				typeCode=0x03;
				paramCode=0x8c;  //continue fast answer
			}
			else
			{
				typeCode=0x03;
				paramCode=0x86; //continue rush
			}
		}
		else if(params.param1==1)
		{
			if(params.param2==1)
			{
				typeCode=0x03;
				paramCode=0x83; //start new rush quiz without answer
			}
			else if(params.param2==2)
			{
				typeCode=0x03;
				paramCode=0x84;//start new rush quiz with answer
			}
			else if(params.param2==3)
			{
				typeCode=0x03;
				paramCode=0x8b;//start new fast answer
			}
		}
	}
	else if (type==QT_Single)
	{
		typeCode=0x03;
		paramCode=0x8a;// start all answer;
	}
	else if (type==QT_SelectId)
	{
		if (params.param2==1)
		{
			typeCode=0x04;
			paramCode=0x80 | params.param1;  //set winnor without answer
		}
		else if(params.param2==2)
		{
			typeCode=0x05;
			paramCode=0x80 | params.param1; //set winnor with answer
		}
		else if(params.param2==3)
		{
			typeCode=0x06;
			paramCode=0x80 | params.param1; //set right
		}
		else if(params.param2==4)
		{
			typeCode=0x07;
			paramCode=0x80 | params.param1; //set wrong
		}

	}
	else if (type==QT_Control)
    {
        if (params.param1==1)
		{
			typeCode=0x03;
			paramCode=0x88;  //music on
		}
        else if (params.param1==2)
		{
			typeCode=0x03;
			paramCode=0x89; // music off
		}
        else if (params.param1==3)
		{
			typeCode=0x03;
			paramCode=0x82; // power off
		}
    }

	if (typeCode!=0)
    {
        unsigned char cmdStart[] = {typeCode, paramCode};
        if (_sendCommand(cmdStart, sizeof(cmdStart)))
        {
            _type = type;
            return true;
        }
    }
    return false;
}

bool RF215::startQuizDirectly(RF21xQuizType type, void *buffer)
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

   // unsigned char cmdStart[] = {0x5A, 0x80};
    return false;// _sendCommand(cmdStart, sizeof(cmdStart));
	}
}

bool RF215::stopQuiz()
{
    unsigned char cmdStart[] = {0x03, 0x85};
    _sendCommand(cmdStart, sizeof(cmdStart));
    return true;
}

bool RF215::setKeypadId(int id)
{
    unsigned char cmd[2] = { 0x02, 0x80 | id};
    _sendCommand(cmd, sizeof(cmd));

    return true;
}

unsigned char RF215::GetQuizResult(int id,int QuizNo)
{
	{
		return 0;
	}
}

void RF215::ClearQuizResult()
{
}

bool RF215::_sendCommand(unsigned char *cmd, int cmdLength,
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


