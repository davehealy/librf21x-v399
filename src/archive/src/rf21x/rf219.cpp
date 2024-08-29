#include "rf219.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

char AllIdQuData[100][110];

void RF219::dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
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

bool RF219::_checkPackage(unsigned char *data, int retLength)
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

void RF219::_processPackage(unsigned char *data, int length)
{
    static std::vector<std::string> textBuf(2400, "");
	std::string answer = "";
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
	else if(data[3]==0x79) //read receiver string
	{
		for(int i=1;i<=20;i++)
		{
			answer+=char(data[3+i]);
		}
		message.messageType=MT_RecString;
		message.data=answer;
	}
	else if(data[3]==0x29) //read version
	{
		answer+=char(data[4]/16+0x30);
		answer+=".";
		answer+=char(data[4]%16+0x30);
		message.messageType=MT_RecVersion;
		message.data=answer;
	}
	else if(data[3]==0x2d)  //read channel
	{
		if(data[4]>=32)
		{
			answer+="0";
		}
		else
		{
			answer+=char(0x30+(data[4]+1)/10);
			answer+=char(0x30+(data[4]+1)%10);
		}
		message.messageType=MT_RecChannel;
		message.data=answer;
	}
	else if(data[3]==0x2e)  //read power
	{
		if (data[4]==198)
		{
			answer+="1";
		}
		else if(data[4]==127)
		{
			answer+="2";
		}
		else if(data[4]==254)
		{
			answer+="3";
		}
		else
		{
			answer+="0";
		}
		message.messageType=MT_RecPower;
		message.data=answer;
	}
	else if(data[3]==0x76) //read rec sn
	{
		for(int i=1;i<=8;i++)
		{
			answer+=char(0x30+data[3+i]);
		}
		message.messageType=MT_RecSn;
		message.data=answer;
	}
	else if (length == 6 && data[3] == 0x34)
    {
        message.messageType = MT_Teacher;
        int key = data[4] & 0x7F;
        if (key >= 0 && key < 12)
        {
            message.data += _teacherMessagesString[key];
        }
    }
    else if (length >= 8 && data[3] == 0x32)
    {
        message.messageType = MT_Student;
		std::string answer = "";

		if(_Is303s)
		{
			message.keypadId = data[4] +1;//+ In_Buff[mComNo, 5] * 16 + 1;// '8
			unsigned char AnByte,FF,LL,RR,GG,BB;
			AnByte=data[5] / 16;
			FF=data[6] / 128;
			LL=(data[6] % 128)/ 64;
			RR=(data[6] % 64)/ 16;
			GG=(data[6] % 16)/ 4;
			BB=data[6] % 4;
			answer=char('A'+AnByte-1); //2位数字字符表示ID
			answer=answer+'F'+char(FF+'0')+ 'L' + char(LL+'0')+'R'+char(RR+'0')+'G'+char(GG+'0')+'B'+char(BB+'0') ;
	//		int ts=(data[9]*256+data[8])*256+data[7];
			message.TimeStamp=(data[9]*256+data[8])*256+data[7];   //reset time should get from saveRawData
		//	int tt=(data[12]*256+data[11])*256+data[10];
//			answer=answer + 'S'+ inttostr(ts);
	//		answer=answer + 'T'+ inttostr(tt);
			message.data = answer;
		}
	  else
	  {
        message.keypadId = (data[4] & 0x7F) + 8 * data[5];
	
		if(_type==QT_Quiz209)
		{
			message.quizNumber=data[7]+1;
			if((message.quizNumber ==104)||(message.quizNumber==106)) //battery level
			{
				answer+=char(data[6]+0x30);
				if ((message.keypadId<101) && (message.keypadId>0) && (message.quizNumber <111) && (message.quizNumber>0) )
				{
					AllIdQuData[message.keypadId-1][message.quizNumber-1]=char(data[6]+0x30);
				}
			}
			else
			{
				answer+=char(data[6]+0x40); //answer
				if ((message.keypadId<101)&&(message.keypadId >0) && (message.quizNumber <111)&& (message.quizNumber >0))
				{
					AllIdQuData[message.keypadId-1][message.quizNumber-1]=char(data[6]+0x40);
				}
			}
			message.data=answer;
		}
        // single selection
        else if (_type == QT_Single)
        {
            answer = char('A' + data[6] - 1);
            if (length >= 10)
			{
				message.TimeStamp=(data[7]*256+data[8])*256+data[9];
			}
        }

        // multiple selection
        else if (_type == QT_Multiple)
        {
            for (int i = 0; i < 8; ++i)
            {
                if ((data[6] & (1 << i)) != 0)
                {
                    answer += char('A' + i);
                }
            }
        }

        // number
        else if (_type == QT_Number)
        {
            for (int i = 7; i < length - 1; ++i)
            {
                int v = data[i];
                if (v >= 0xF4)
                {
                    // End of message
                    break;
                }
                else if (v <= 0x60)
                {
                    // Contents, converted to ASCII code by plus 0x20
                    answer += (char)(0x20 + v);
                }
                else if (v == 0xF0)
                {
                    // Denominator follow
                    answer += " _Denominator__";
                }
                else if (v == 0xF1)
                {
                    // Molecular follow
                    answer += " _Molecular__";
                }
            }
        }

        // text
        else if (_type == QT_Text)
        {
            for (int i = 7; i < length - 2; ++i)
            {
                int v = data[i];
                if (v >= 0xF4)
                {
                    // End of message
                    continue;
                }
                else if (v <= 0x60)
                {
                    // Contents, converted to ASCII code by plus 0x20
                    answer += (char)(0x20 + v);
                }
            }
            if ((data[length - 2] & 0x80) == 0)
            {
                textBuf[message.keypadId] += answer;
                answer = "";
            }
            else
            {
                answer = textBuf[message.keypadId] + answer;
                textBuf[message.keypadId] = "";
            }
        }

        // classify
        else if (_type == QT_Classify)
        {
            int n = 0;
            for (int i = 7; i < 7 + 12; i += 2)
            {
                answer += char('1' + n);
                answer += '.';
                ++n;
                for (int j = 0; j < 8; ++j)
                {
                    if ((data[i] & (1 << j)) != 0)
                    {
                        answer += char('A' + j);
                    }
                }
                for (int j = 8; j < 10; ++j)
                {
                    if ((data[i + 1] & (1 << (j - 8))) != 0)
                    {
                        answer += char('A' + j);
                    }
                }
                if (i < 7 + 12 + 2)
                {
                    answer += ' ';
                }
            }
        }

        // sort
        else if (_type == QT_Sort)
        {
            for (int i = 7; i < 7 + 9; ++i)
            {
                if (data[i] != 0)
                {
                    answer += char('A' + data[i] - 1);
                }
            }
        }

        // judge Or vote
        else if (_type == Qt_JudgeOrVote)
        {
            answer += char('A' + data[6] - 1);
        }

        // homework
        else if ((_type == QT_Homework) || (_type == QT_SelfPaced))
        {
            message.quizNumber = data[6];

            for (int i = 7; i < length - 1; ++i)
            {
                int v = data[i];
                if (v >= 0xF4)
                {
                    // End of message
                    continue;
                }
                else if (v <= 0x60)
                {
                    // Contents, converted to ASCII code by plus 0x20
                    answer += (char)(0x20 + v);
                }
            }
        }

        // rush
        else if (_type == QT_Rush)
        {
            answer += "rush";
        }

        // unknow
        else if (_type == QT_Unknow)
        {
        }
        message.data = answer;
	  }
    }
    else
    {
        return;
    }
	
    this->_executeMessageEvent(message);
}

RF219::RF219()
{
    _type = QT_Unknow;
}

RF219::~RF219()
{
    close();
}

bool RF219::open(const std::string port, int minId, int maxId)
{
    if (_sp.isOpened() == false)
    {
        close();
    }

    bool ret = 0;
	unsigned char cmdOpen[] = {0x12, 0};
	unsigned char retOpen[4 + 2];

	unsigned char cmdConnect303s[] = {0x10, 0x00 };
	unsigned char retConnect303s[4+2] = {0};

	if (_sp.open(port, 750000) == false)
	{
		goto failsafe_219;
	}
	_sp.setRecvEvent(this);

	ret = _sendCommand(cmdConnect303s, sizeof(cmdConnect303s),
		100,
		retConnect303s, sizeof(retConnect303s));
	if (!ret || retConnect303s[3+0] != 0x10)
	{
		goto failsafe_219;
	}
	//printf("cmdConnect 115200\n");
	_Is303s=true;
	if (maxId<=0|| maxId>64)
	{
		return false;
	}
	unsigned char cmdInitId[]={0x14,char(maxId-1)};
	ret=_sendCommand(cmdInitId,2,100,retOpen,sizeof(retOpen));

	ret=_sendCommand(cmdOpen,sizeof(cmdOpen),200,retOpen,sizeof(retOpen));
	//stopQuiz();
	if (!ret || retOpen[3 + 0] != 0x12
		|| retOpen[3 + 1] != 1)
	{
		goto failsafe;
	}

	return true;

failsafe_219:
	_Is303s=false;


    unsigned char cmdConnect[] = {0x10, 0};
    unsigned char retConnect[4 + 2] = {0};
    unsigned char cmdSet[] = {0x14, minId % 8, minId / 8, maxId % 8, maxId / 8};
    unsigned char retSet[4 + 1];

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
	_type=QT_Quiz209;
  //  stopQuiz();

    return true;

failsafe:
    close();
    return false;
}

bool RF219::close()
{
 /*   stopQuiz();

    unsigned char cmd[] = {0x11};
    _sendCommand(cmd, sizeof(cmd));

    cmd[0] = 0x13;
    _sendCommand(cmd, sizeof(cmd));
	*/
    _sp.close();

    return true;
}

bool RF219::startQuiz(RF21xQuizType type, const RF21xQuizParams &params)
{
    int typeCode = 0;
    int paramCode = 0;

	if (_Is303s)
	{
		if (type == QT_Rush || type == QT_Single)
		{
			unsigned char cmd303sStart[] = {0x85};
			if (_sendCommand(cmd303sStart, sizeof(cmd303sStart)))
			{
				_type = type;
				return true;
			}
		}
		return false;
	}

    if (type == QT_Single)
    {
        typeCode = 0;
        paramCode = 8;
        if (params.param1 >= 1 && params.param1 <= 10)
        {
            paramCode = params.param1; // total options
        }
    }
    else if (type == QT_Multiple)
    {
        typeCode = 4;
        paramCode = 8;
        if (params.param1 >= 1 && params.param1 <= 8)
        {
            paramCode = params.param1; // total options
        }
        if (params.param2 >= 1 && params.param1 <= 4)
        {
            typeCode = params.param2 - 1; // total right options
        }
    }
    else if (type == QT_Number)
    {
        typeCode = 8;
        paramCode = 1;
    }
    else if (type == QT_Text)
    {
        typeCode = 7;
        paramCode = 120;
        if (params.param1 >= 1 && params.param1 <= 120)
        {
            paramCode = params.param1; // max length
        }
    }
    else if (type == QT_Classify)
    {
        typeCode = 6;
        int options = 10;
        int classes = 6;
        if (params.param1 >= 1 && params.param1 <= 10)
        {
            options = params.param1; // total options
        }
        if (params.param2 >= 1 && params.param1 <= 6)
        {
            classes = params.param2; // total classes
        }
        paramCode = options | (classes << 4);
    }
    else if (type == QT_Sort)
    {
        typeCode = 5;
        paramCode = 9;
        if (params.param1 >= 1 && params.param1 <= 9)
        {
            paramCode = params.param1; // total options
        }
    }
    else if (type == Qt_JudgeOrVote)
    {
        typeCode = 13;
        paramCode = 3;
        if (params.param1 >= 1 && params.param1 <= 3)
        {
            paramCode = params.param1; // total options
        }
        if (params.param2 >= 10 && params.param2 <= 15)
        {
            typeCode = params.param2; // display type
        }
    }
	else if (type== QT_SelfPaced)
	{
		int totalNumber = params.param1;
		if (totalNumber < 1 || totalNumber > 100)
		{
			totalNumber = 100;
		}
		typeCode = 0x1e;
		paramCode = totalNumber;
	}
	else if (type == QT_Homework)
    {
        int totalNumber = params.param1;
        int subject = params.param2;
        if (totalNumber < 1 || totalNumber > 100)
        {
            totalNumber = 100;
        }
        if (subject < 1 || subject > 7)
        {
            subject = 1;
        }
        typeCode = 0x10 + subject;
        paramCode = totalNumber;
    }
    else if (type == QT_Rush)
    {
        typeCode = 0x1F;
        paramCode = 1;
    }
    else if (type==QT_Quiz209)
	{
		int totalNumber = params.param2;
		if (totalNumber < 1 || totalNumber > 100)
		{
			totalNumber = 100;
		}
		typeCode = 0x00;
		paramCode = totalNumber;
	}

	else
    {
        return false;
    }

    unsigned char typeCont = _QuizFlag;
	if (params.newQuiz==1)
	{
		if (typeCont==0x00)
		{
			_QuizFlag=0x20;
		}
		else
		{
			if (typeCont==0x20)
			{
				_QuizFlag=0x40;
			}
			else
			{
				if(typeCont==0x40)
				{
					_QuizFlag=0x60;
				}
				else
				{
					_QuizFlag=0x00;
				}
			}
		}
	}
	else
	{
		if((typeCont!=0x00) && (typeCont!=0x20) && (typeCont!=0x40) && (typeCont!=0x60))
		{
			_QuizFlag=0x00;
		}
	}

    unsigned char cmdStart[] = {0x15, typeCode|_QuizFlag, paramCode};
    unsigned char retStart[4 + 2];

	if (type==QT_Quiz209) //209不等了，因为可能会马上有电池信息收到
	{
		_type=type;
		return _sendCommand(cmdStart,sizeof(cmdStart));
	}

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

bool RF219::startQuizDirectly(RF21xQuizType type, void *buffer)
{
	unsigned char *code = (unsigned char*)buffer;
	unsigned char cmdStart[255];
	int i;

	std::string answer = "";
	RF21xMessage message;
	message.quizType = _type;
	//message.saveRawData(data, length);
	message.TimeStamp = 0;
	if(_Is303s)
	{
		if(type==QT_Control)
		{
			if(code[0]==1) // sleep
			{
				cmdStart[0]=0x16;
				_sendCommand(cmdStart,1);
				_sp.setTimeout(30);
				cmdStart[0]=0x72;
				cmdStart[1]=code[1];
				cmdStart[2]=ColorToByte(0,code[3],code[4],code[5],code[6]);
				cmdStart[3]=code[2];
				return _sendCommand(cmdStart,4);
			}
			else if(code[0]==2) //awake
			{
				cmdStart[0]=0x16;
				_sendCommand(cmdStart,1);
				_sp.setTimeout(30);
				cmdStart[0]=0x73;
				cmdStart[1]=ColorToByte(0,code[1],code[2],code[3],code[4]);
				return _sendCommand(cmdStart,2);
			}
			else if (code[0]==3) //update one id
			{
				//function UpdateOneId(mBase:byte;LedId:Integer;Flag:Byte; Lum:Byte;mR:Byte;mG:Byte;mB:Byte):integer;
				if(code[1]>=64)
				{
					return false;
				}
				cmdStart[0]=0x2d;

				unsigned tm,td,tnew; //303S需要对ID作一个转换
				tm=code[1] %4;
				td=code[1] /4;
//				tnew= tm *16 +td;

				cmdStart[1]= tm ;//tnew / 16;
				cmdStart[2]= td ;//tnew % 16 ;
				cmdStart[3]=ColorToByte(code[2],code[3],code[4],code[5],code[6]);
				return _sendCommand(cmdStart,4);
			}
			else if (code[0]==4) // update all same
			{
//				function UpdateAllIdSame(mBase:byte;Flag:Byte; Lum:Byte;mR:Byte;mG:Byte;mB:Byte):integer;
				cmdStart[0]=0x74;
				cmdStart[1]=ColorToByte(code[1],code[2],code[3],code[4],code[5]);
				return _sendCommand(cmdStart,2);
			}
			else if (code[0]==5) // setall lum
			{
				cmdStart[0]=0x84;
				cmdStart[1]=code[1];
				return _sendCommand(cmdStart,2);
			}
			else if (code[0]==6) //ResetTime
			{
				cmdStart[0]=0x26;
				return _sendCommand(cmdStart,1);
			}
			else if (code[0]==7)//start ping
			{
				cmdStart[0]=0x86;
				return _sendCommand(cmdStart,1);
			}
			else if(code[0]==8) //open base
			{
				cmdStart[0]=0x12;
				cmdStart[1]=0;
				return _sendCommand(cmdStart,2);
			}
			else if(code[0]==9) //close base
			{
				cmdStart[0]=0x13;
				return _sendCommand(cmdStart,1);
			}
			else if(code[0]==10)//stop base
			{
				cmdStart[0]=0x16;
				return _sendCommand(cmdStart,1);
			}

		}
		return false;
	}


	if(type==QT_Control)
	{
		if (code[0]==1) //set channel
		{
			if (code[1]==0)
			{
				cmdStart[0]=0x2c;
				cmdStart[1]=32;
				return _sendCommand(cmdStart,2);
			}
			else if (code[1]<33)
			{
				cmdStart[0]=0x2c;
				cmdStart[1]=code[1]-1;
				return _sendCommand(cmdStart,2);
			}
			else
			{
				return false;
			}
		}
		else if (code[0]==2) // read channel
		{
			cmdStart[0]=0x2d;
			return _sendCommand(cmdStart,1);
		}
		else if (code[0]==3) //set power
		{
			if (code[1]==1) //low power
			{
				cmdStart[0]=0x2A;
				cmdStart[1]=198;
				return _sendCommand(cmdStart,2);
			}
			else if(code[1]==2) // middle power
			{
				cmdStart[0]=0x2A;
				cmdStart[1]=127;
				return _sendCommand(cmdStart,2);
			}
			else if(code[1]==3) // high power
			{
				cmdStart[0]=0x2A;
				cmdStart[1]=254;
				return _sendCommand(cmdStart,2);
			}
			else
			{
				return false;
			}
		}
		else if(code[0]==4) // read power
		{
				cmdStart[0]=0x2e;
				return _sendCommand(cmdStart,1);
		}
		else if (code[0]==5) // power off all keypads
		{
			cmdStart[0]=0x15;
			cmdStart[1]=0x09;
			cmdStart[2]=0x47;
			return _sendCommand(cmdStart,3);
		}
		else if(code[0]==6) // write receiver name
		{
			cmdStart[0]=0x78;
			for(i=1;i<21;i++)
			{
				cmdStart[i]=code[i];
			}
			return _sendCommand(cmdStart,21);
		}
		else if(code[0]==7) // read receiver name
		{
			cmdStart[0]=0x79;
			return _sendCommand(cmdStart,1);
		}
		else if(code[0]==8) // read receiver sn
		{
			cmdStart[0]=0x76;
			return _sendCommand(cmdStart,1);
		}
		else if(code[0]==9) //read receiver version
		{
			cmdStart[0]=0x29;
			return _sendCommand(cmdStart,1);
		}
		else if(code[0]==10) //set can sleep
		{
			cmdStart[0]=0x7a;
			cmdStart[1]=1;
			return _sendCommand(cmdStart,2);
		}
		else if(code[0]==11) //set can power off
		{
			cmdStart[0]=0x7b;
			cmdStart[1]=1;
			return _sendCommand(cmdStart,2);
		}
		else if(code[0]==12) //set can not sleep
		{
			cmdStart[0]=0x7a;
			cmdStart[1]=0;
			return _sendCommand(cmdStart,2);
		}
		else if(code[0]==13) //set can not power off
		{
			cmdStart[0]=0x7b;
			cmdStart[1]=0;
			return _sendCommand(cmdStart,2);
		}
		else if(code[0]==14) //ping
		{
			cmdStart[0]=0x15;
			cmdStart[1]=0x09;
			cmdStart[2]=0x38;
			return _sendCommand(cmdStart,3);
		}
		else if(code[0]==15) // set channel of keypads on line
		{
			if (code[1]==0)
			{
				cmdStart[0]=0x15;
				cmdStart[1]=0x0a;
				cmdStart[2]=64;//32;
				return _sendCommand(cmdStart,3);
			}
			else if (code[1]<33)
			{
				cmdStart[0]=0x15;
				cmdStart[1]=0x0a;
				cmdStart[2]=(code[1]-1)*2;
				return _sendCommand(cmdStart,3);
			}
			else
			{
				return false;
			}
		}
		else if (code[0]==16) //获取指定ID所有题目结果
		{
			if ((code[1]<101) && (code[1]>0))
			{
				message.messageType=MT_AllQuResult;
				message.keypadId =code[1];
				message.quizNumber=0;
				for(i=0;i<110;i++)
				{
					answer+=AllIdQuData[code[1]-1][i];
					answer+=",";
				}
				message.data=answer;
				this->_executeMessageEvent(message);
			}
		}
		else if (code[0]==17) //获取指定题号的所有ID的结果
		{
			if((code[1]<111) && (code[1]>0))
			{
				message.messageType=MT_AllIdResult;
				message.keypadId=0;
				message.quizNumber=code[1];
				for(i=0;i<100;i++)
				{
					answer+=AllIdQuData[i][code[1]-1];
					answer+=",";
				}
				message.data=answer;
				this->_executeMessageEvent(message);
			}			
		}
		else if (code[0]==18) //清除所有记录
		{
			for(i=0;i<100;i++)
			{
				for (int j=0;j<110;j++)
				{
					AllIdQuData[i][j]= 0x20;// " ";
				}
			}
		}
	}
	else if(type==QT_SendMessage)
	{
		if (code[0]>100||code[1]>=3)
		{
			return false;
		}
		else
		{
			cmdStart[0]=0x25;
			cmdStart[1]=code[0];
			cmdStart[2]=code[1];
			for(i=2;i<28;i++)
			{
				if ((code[i]>=32) && (code[i]<=0x8d))
				{
					cmdStart[i+1]=code[i]-32;
				}
				else
				{
					cmdStart[i+1]=0xff;
				}
			}
			return _sendCommand(cmdStart,29);
		}
	}
	else if (type==QT_UserDefine) //自定义发送数据，可以考虑其它型号都增加这一功能
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
	}
    return true;
}

bool RF219::stopQuiz()
{
	if (_Is303s)
	{
		unsigned char cmd303Stop[] = {0x87};
		if (_sendCommand(cmd303Stop, sizeof(cmd303Stop)))
		{
			return true;
		}
	}
	return false;

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

bool RF219::setKeypadId(int id)
{
	if(_Is303s)
	{
		if (id<0|| id>63)
		{
			return false;
		}
		else
		{
			unsigned char cmdSetId[]={0x17,char(id),0,0,0,0};
			_sendCommand(cmdSetId,6);
			return true;
		}
	}

    unsigned char cmd[3];

    if (id == 0)
    {
        cmd[0] = 0x24;
        _sendCommand(cmd, 1);
    }
    else if (id > 0)
    {
        cmd[0] = 0x17;
        cmd[1] = id % 8;
        cmd[2] = id / 8;
        _sendCommand(cmd, 3);
    }
    else
    {
        return false;
    }

    return true;
}

unsigned char RF219::GetQuizResult(int id,int QuizNo)
{
	if ((id<101) && (id>0) && (QuizNo<111) && (QuizNo>0))
	{
		return AllIdQuData[id-1][QuizNo-1];
	}
	else
	{
		return 0;
	}
}

void RF219::ClearQuizResult()
{
	for (int i=0;i<101;i++)
	{
		for (int j=0;j<111;j++)
		{
			AllIdQuData[i][j]=0x20;
		}
	}
}

unsigned char RF219::ColorToByte(unsigned char Flag,unsigned char Lum,unsigned char mR,unsigned char mG,unsigned char mB)
{
	unsigned char result=0;
	if ((Flag>1) || (Lum>1) || (mR>3) || (mG>3)|| (mB>3))
	{
		return result;
	}
	result=Flag*128+Lum * 64 + mR * 16 + mG * 4 + mB;
	return result;
}


bool RF219::readDeviceSerialNumber(const std::string port, std::string &sn, bool isDefault)
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

bool RF219::writeDeviceSerialNumber(const std::string port, const std::string sn)
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

bool RF219::resetDeviceSerialNumber(const std::string port)
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

bool RF219::checkDeviceSerialNumber(const std::string port, const std::string sn)
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

bool RF219::_sendCommand(unsigned char *cmd, int cmdLength,
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
		tthread::this_thread::sleep_for(tthread::chrono::milliseconds(200));
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


