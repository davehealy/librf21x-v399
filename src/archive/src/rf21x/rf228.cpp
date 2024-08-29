#include "rf228.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

void RF228::dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
{
    //printf("event\n");
    static unsigned char buf[GSP_MAX_DATA_SIZE + 1000];
    static int len = 0;
    memcpy(buf + len, data, length);
    len += length;

    int i = 0;
    int index = 0;
    while (i < len)
    {
        unsigned char *packageData = buf + i;
        int packageLength = buf[i + 2] + 4;
        if (i + packageLength <= len && _checkPackage(packageData, packageLength))
        {
            _processPackage(packageData, packageLength);
            i += packageLength;
            index = i;
        }
        else
        {
            ++i;
        }
    }
    memcpy(buf, buf + index, len - index);
    len = len - index;
}

bool RF228::_checkPackage(unsigned char *data, int retLength)
{
    if (data[0] != 0x27 || data[1] != 0xA5)
    {
        return false;
    }
    if (data[2] != retLength - 4)
    {
        return false;
    }
    int v = 0;
    for (int i = 3; i < retLength - 1; ++i)
    {
        v = v ^ data[i];
    }
    if (data[retLength - 1] != v)
    {
        return false;
    }
    return true;
}

void RF228::_processPackage(unsigned char *data, int length)
{
    static std::vector<std::string> textBuf(400, "");

    RF21xMessage message;
    message.quizType = _type;
    message.saveRawData(data, length);
    message.TimeStamp = 0;

//    for (int i = 0; i < length; ++i)
//    {
//        printf("package data %d: %X\n", i, data[i]);
//    }

    if ((length == 6 && data[3] == 0x17) || (length == 6 && data[3] == 0x24))
    {
        message.messageType = MT_SetId;
        if (data[4] != 0)
        {
            message.data = "Success";
        }
        else
        {
            message.data = "Fail";
        }
    }
    else if (data[3] == 0x32)
    {
        message.keypadId = data[5] + 256 * (data[6] + 256 * data[7]);
        if (message.keypadId == 0)
        {
            message.messageType = MT_Teacher;
            int key = data[8] & 0x7F;
            if (key >= 0 && key < 12)
            {
                message.data += _teacherMessagesString[key];
            }
        }
        else
        {
            message.messageType = MT_Student;
            if (_isSelfPaced)
            {
                message.quizNumber = data[data[2] + 1];
            }
            else
            {
                message.quizNumber = 0;
            }

            std::string answer = "";
            int b[7];
            int pp = -1;
            switch (_type)
            {
            case QT_Single:
				b[0]=data[8] & 0x0f;
				if (b[0]==0x00)
				{
					answer="J";
				}
				else if (b[0]==0x0b)
				{
					answer="Rush";
				}
				else if (b[0]<=0x09)
				{
					answer = char('A' + (0x0F & data[8]) - 1);
				}
                break;
            case QT_Multiple:
                b[0] = 0x0F & data[9];
                b[1] = (0xF0 & data[10]) / 16;
                b[2] = 0x0F & data[10];
                b[3] = (0xF0 & data[11]) / 16;
                b[4] = 0x0F & data[11];
                b[5] = (0xF0 & data[8]) / 16;
                b[6] = 0x0F & data[8];
                for (int i = 0; i < 7; ++i)
                {
					if (b[i]==0x00)
					{
						answer+="J";
					}
					else if (b[i]==0x0b)
					{
						answer+="Rush";
					}
					else if (b[i] >= 0 && b[i] <=9)// 7)
                    {
                        answer += char('A' + b[i] - 1);
                    }
                }
                break;
            case QT_Number:
                if (0x80 & data[9])
                {
                    pp = (0x70 & data[9]) >> 4;
                    pp = 7 - pp - 1;
                }
                else
                {
                    pp = -1;
                }
                b[0] = 0x0F & data[9];
                b[1] = (0xF0 & data[10]) / 16;
                b[2] = 0x0F & data[10];
                b[3] = (0xF0 & data[11]) / 16;
                b[4] = 0x0F & data[11];
                b[5] = (0xF0 & data[8]) / 16;
                b[6] = 0x0F & data[8];
                for (int i = 0; i < 7; ++i)
                {
                    if (b[i] >= 0 && b[i] <= 9)
                    {
                        answer += char('0' + b[i]);
                    }
                    else if (b[i] == 0x0A)
                    {
                        answer += char('-');
                    }
					else if (b[i]==0x0b)
					{
						answer += "Rush";
					}

                    if (i == pp)
                    {
                        answer += char('.');
                    }
                }
                break;
            default:
                break;
            }
            message.data = answer;
        }
    }
    else if (data[3] == 0x33)
    {
        message.keypadId = data[11] + 256 * (data[12] + 256 * data[13]);
        if (message.keypadId == 0)
        {
            message.messageType = MT_Teacher;
            int key = data[8] & 0x7F;
            if (key >= 0 && key < 12)
            {
                message.data += _teacherMessagesString[key];
            }
        }
        else
        {
            message.messageType = MT_Auth;

            int ch = 0x03 & data[4];
            _channels[message.keypadId] = ch;

            int user = data[5] + 256 * (data[6] + 256 * data[7]);
            message.quizNumber = user;

            std::string pwd;
            int b[6];
            b[0] = (0xF0 & data[9]) / 16;
            b[1] = 0x0F & data[9];
            b[2] = (0xF0 & data[10]) / 16;
            b[3] = 0x0F & data[10];
            b[4] = (0xF0 & data[8]) / 16;
            b[5] = 0x0F & data[8];
            for (int i = 0; i < 7; ++i)
            {
                if (b[i] >= 0 && b[i] <= 9)
                {
                    pwd += char('0' + b[i]);
                }
            }
            message.data = pwd;
        }
    }

    this->_executeMessageEvent(message);
}

RF228::RF228()
{
    _type = QT_Unknow;
    _isSelfPaced = false;
}

RF228::~RF228()
{
    close();
}

bool RF228::open(const std::string port, int minId, int maxId)
{
    if (_sp.isOpened() == false)
    {
        close();
    }

    bool ret = 0;

    unsigned char cmdConnect[] = {0x10, 0};
    unsigned char retConnect[4 + 2] = {0};
    unsigned char cmdSet[] = {0x14, minId % 8, minId / 8, maxId % 8, maxId / 8};
    unsigned char retSet[4 + 1];
    unsigned char cmdOpen[] = {0x12, 0};
    unsigned char retOpen[4 + 2];

    if (_sp.open(port, 256000) == false)
    {
        goto failsafe;
    }
    _sp.setRecvEvent(this);

    ret = _sendCommand(cmdConnect, sizeof(cmdConnect),
                       500,
                       retConnect, sizeof(retConnect));
    if (!ret || retConnect[3 + 0] != 0x10)
    {
        goto failsafe;
    }
    //printf("cmdConnect\n");

    ret = _sendCommand(cmdSet, sizeof(cmdSet),
                       500,
                       retSet, sizeof(retSet));
    if (!ret || retSet[3 + 0] != 0x14)
    {
        goto failsafe;
    }
    //printf("cmdSet\n");

    ret = _sendCommand(cmdOpen, sizeof(cmdOpen),
                       2000,
                       retOpen, sizeof(retOpen));
    if (!ret || retOpen[3 + 0] != 0x12
             || retOpen[3 + 1] != 1)
    {
        goto failsafe;
    }
    //printf("cmdOpen");

    stopQuiz();

    return true;

failsafe:
    close();
    return false;
}

bool RF228::close()
{
    stopQuiz();

    unsigned char cmd[] = {0x13};
    _sendCommand(cmd, sizeof(cmd));

    cmd[0] = 0x11;
    _sendCommand(cmd, sizeof(cmd));

    _sp.close();

    return true;
}

bool RF228::startQuiz(RF21xQuizType type, const RF21xQuizParams &params)
{
    if (type == QT_Notification)
    {
        unsigned char cmd[5];
        cmd[0] = 0x2d;
        cmd[1] = _channels[params.param1];
        if (params.param2)
        {
            cmd[1] |= 0xd0;
        }
        else
        {
            cmd[1] |= 0xe0;
        }
        int sn = params.param1;
        cmd[2] = 0xFF & (sn >> 0);
        cmd[3] = 0xFF & (sn >> 8);
        cmd[4] = 0xFF & (sn >> 16);
        _sendCommand(cmd, 5);
        return true;
    }

    int typeCode = 0;
    if (type == QT_Single || type == QT_Rush)
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
    else
    {
        return false;
    }
    typeCode = typeCode << 4;

    int paramCode = 0;
    if (params.param1 > 0)
    {
        paramCode = params.param1;
        typeCode |= 0x80;
        _isSelfPaced = true;
    }
    else
    {
        _isSelfPaced = false;
    }

    unsigned char cmdStart[] = {0x15, typeCode, paramCode};
    unsigned char retStart[4 + 2];
    bool ret = _sendCommand(cmdStart, sizeof(cmdStart),
                            100,
                            retStart, sizeof(retStart));

    if (!ret || retStart[3 + 0] != 0x15
             || retStart[3 + 1] != 1)
    {
        return false;
    }
    _type = type;

    return true;
}

bool RF228::startQuizDirectly(RF21xQuizType type, void *buffer)
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
		unsigned char cmdStart[] = {0x15, code[0], code[1]};
		unsigned char retStart[4 + 2];
		bool ret = _sendCommand(cmdStart, sizeof(cmdStart),
								100,
								retStart, sizeof(retStart));

		if (!ret || retStart[3 + 0] != 0x15
				 || retStart[3 + 1] != 1)
		{
			return false;
		}
		_type = type;

		return true;
	}
}

bool RF228::stopQuiz()
{
    unsigned char cmdStop[] = {0x16};
    unsigned char retStop[4 + 1];
    bool ret = _sendCommand(cmdStop, sizeof(cmdStop),
                            100,
                            retStop, sizeof(retStop));

    if (!ret || retStop[3 + 0] != 0x16)
    {
        return false;
    }

    return true;
}

bool RF228::setKeypadId(int id)
{
    unsigned char cmd[4];

    if (id == 0)
    {
        cmd[0] = 0x24;
        _sendCommand(cmd, 1);
    }
    else if (id > 0)
    {
        cmd[0] = 0x17;
        cmd[1] = 0xFF & (id >> 0);
        cmd[2] = 0xFF & (id >> 8);
        cmd[3] = 0xFF & (id >> 16);
        _sendCommand(cmd, 4);
    }
    else
    {
        return false;
    }

    return true;
}

unsigned char RF228::GetQuizResult(int id,int QuizNo)
{
	{
		return 0;
	}
}

void RF228::ClearQuizResult()
{
}


bool RF228::readDeviceSerialNumber(const std::string port, std::string &sn, bool isDefault)
{
    if (_sp.isOpened() == false)
    {
        close();
        if (_sp.open(port, 256000) == false)
        {
            return false;
        }
    }

    unsigned char cmd[2];
    cmd[0] = 0x76;
    if (isDefault)
    {
        cmd[1] = 0;
    }
    else
    {
        cmd[1] = 1;
    }

    unsigned char ret[4 + 9];
    bool isOk = _sendCommand(cmd, sizeof(cmd),
                            100,
                            ret, sizeof(ret));

    if (!isOk || ret[3 + 0] != 0x76)
    {
        return false;
    }

    sn = "";
    for (int i = 0; i < 8; ++i)
    {
        char buf[10];
        unsigned char t = ret[i+4];
        sprintf(buf, "%x", t);
        sn += buf;
    }

    return true;
}

bool RF228::writeDeviceSerialNumber(const std::string port, const std::string sn)
{
    if (sn.length() < 8)
    {
        return false;
    }

    if (_sp.isOpened() == false)
    {
        close();
        if (_sp.open(port, 256000) == false)
        {
            return false;
        }
    }

    unsigned char cmdWrite[9];
    cmdWrite[0] = 0x75;
    for (int i = 0; i < 8; ++i)
    {
        unsigned char t = (unsigned char)atoi(sn.substr(i, 1).c_str());
        cmdWrite[i + 1] = t;
    }

    unsigned char retWrite[4 + 1];
    bool isOkWrite = _sendCommand(cmdWrite, sizeof(cmdWrite),
                            300, retWrite, sizeof(retWrite));

    if (!isOkWrite || retWrite[3 + 0] != 0x75)
    {
        return false;
    }


    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(300));


    unsigned char cmdApply[4];
    cmdApply[0] = 0x1B;
    int byteX3 = atoi(sn.c_str());
    byteX3 = byteX3 & 0x7FFFF;
    unsigned char byteL = byteX3 % 128;
    byteX3 = (byteX3 - byteL) / 128;
    unsigned char byteH = byteX3 % 256;
    unsigned char byteU = (byteX3 - byteH) / 256;
    cmdApply[1] = byteU;
    cmdApply[2] = byteH;
    cmdApply[3] = byteL;

    unsigned char retApply[4 + 1];
    bool isOkApply = _sendCommand(cmdApply, sizeof(cmdApply),
                                  300, retApply, sizeof(retApply));

    if (!isOkApply || retApply[3 + 0] != 0x1B)
    {
        return false;
    }


    return true;
}

bool RF228::resetDeviceSerialNumber(const std::string port)
{
    if (_sp.isOpened() == false)
    {
        close();
        if (_sp.open(port, 256000) == false)
        {
            return false;
        }
    }

    unsigned char cmd[1];
    cmd[0] = 0x1C;

    unsigned char ret[4 + 1];
    bool isOkWrite = _sendCommand(cmd, sizeof(cmd),
                                  300,  ret, sizeof(ret));

    if (!isOkWrite || ret[3 + 0] != 0x1C)
    {
        return false;
    }

    return true;
}

bool RF228::checkDeviceSerialNumber(const std::string port, const std::string sn)
{
    if (sn.length() < 8)
    {
        return false;
    }

    if (_sp.isOpened() == false)
    {
        close();
        if (_sp.open(port, 256000) == false)
        {
            return false;
        }
    }

    unsigned char cmd[9];
    cmd[0] = 0x1A;
    for (int i = 0; i < 8; ++i)
    {
        //unsigned char t = (unsigned char)atoi(sn.substr(i, 1).c_str());
        unsigned char t = sn[i];
        cmd[i + 1] = t;
    }

    unsigned char ret[4 + 2];
    bool isOk = _sendCommand(cmd, sizeof(cmd),
                            300,
                            ret, sizeof(ret));

    if (!isOk || ret[3 + 0] != 0x1A || ret[3 + 1] != 0x01)
    {
        return false;
    }

    return true;
}

bool RF228::_sendCommand(unsigned char *cmd, int cmdLength,
                         int waitTime,
                         unsigned char *ret, int retLength,
                         bool checkRet)
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
    data[0] = 0x35;
    data[1] = 0x96;
    data[2] = cmdLength;
    memcpy(data + 3, cmd, cmdLength);

    unsigned char v = 0;
    for (int i = 0; i < cmdLength; ++i)
    {
        v = v ^ cmd[i];
    }
    data[cmdLength + 3] = v;

    if (_sp.writeData(data, cmdLength + 4) != cmdLength + 4)
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

    if (ret != NULL && checkRet && _checkPackage(ret, retLength) == false)
    {
        isOk = false;
        goto finish;
    }

finish:
    _sp.enableRecvEvent();
    _sp.setTimeout(50);
    return isOk;
}


