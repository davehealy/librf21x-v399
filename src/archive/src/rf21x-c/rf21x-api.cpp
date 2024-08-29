#include "rf21x-api.h"
#include "rf21x.h"

#include <string.h>

rf21x_Device API_FUNC rf21x_createDevice(int deviceType)
{
    return (rf21x_Device)createRF21x((RF21xDeviceType)deviceType);
}

void API_FUNC rf21x_destoryDevice(rf21x_Device device)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    destoryRF21x(rf);
}

int API_FUNC rf21x_getHidSerialNumber(int index, char *buffer, int maxLength)
{
    std::string str;
    if (getHidSerialNumber(index, str) && (maxLength > str.length()))
    {
        strcpy(buffer, str.c_str());
        return 1;
    }
    return 0;
}

#ifdef _WIN32
int API_FUNC rf21x_getHidTempPath(int index, char *buffer, int maxLength)
{
    std::string str;
    if (getHidTempPath(index, str) && (maxLength > str.length()))
    {
        strcpy(buffer, str.c_str());
        return 1;
    }
    return 0;
}

int API_FUNC rf21x_checkHidCable(const char *tempPath)
{
    if (checkHidCable(tempPath))
    {
        return 1;
    }
    return 0;
}
#endif

int API_FUNC rf21x_open(rf21x_Device device, const char *port, int minId, int maxId)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->open(port, minId, maxId);
}

int API_FUNC rf21x_close(rf21x_Device device)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->close();
}

int API_FUNC rf21x_startQuiz(rf21x_Device device, int type, int param1, int param2, int newQuiz)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    RF21xQuizParams p;
    p.param1 = param1;
    p.param2 = param2;
	p.newQuiz = newQuiz;
    return rf->startQuiz((RF21xQuizType)type, p);
}

int API_FUNC rf21x_startQuizDirectly(rf21x_Device device, int type, void *buffer)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->startQuizDirectly((RF21xQuizType)type, buffer);
}

int API_FUNC rf21x_stopQuiz(rf21x_Device device)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->stopQuiz();
}

int API_FUNC rf21x_setKeypadId(rf21x_Device device, int id)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->setKeypadId(id);
}

int API_FUNC rf21x_readDeviceSerialNumber(rf21x_Device device, const char *port, char *sn, int length, int isDefault)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    std::string s;
    int isOk = (rf->readDeviceSerialNumber(port, s, isDefault) == true);
    if (isOk && length > 8)
    {
        strcpy(sn , s.c_str());
    }
    return isOk;
}

unsigned char API_FUNC rf21x_GetQuizResult(rf21x_Device device,int id,int QuizNo)
{
	AbstractRF21x *rf=(AbstractRF21x*)device ;
	return rf->GetQuizResult(id,QuizNo);
}

void API_FUNC rf21x_ClearQuizResult(rf21x_Device device)
{
	AbstractRF21x *rf=(AbstractRF21x*)device ;
	rf->ClearQuizResult();
}


int API_FUNC rf21x_writeDeviceSerialNumber(rf21x_Device device, const char *port, const char *sn)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    std::string s;
    s += sn;
    return rf->writeDeviceSerialNumber(port, s);
}

int API_FUNC rf21x_resetDeviceSerialNumber(rf21x_Device device, const char *port)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    return rf->resetDeviceSerialNumber(port);
}

int API_FUNC rf21x_checkDeviceSerialNumber(rf21x_Device device, const char *port, const char *sn)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    std::string s;
    s += sn;
    return rf->checkDeviceSerialNumber(port, s);
}

rf21x_Message API_FUNC rf21x_createMessageBuffer()
{
    return (new RF21xMessage());
}

void API_FUNC rf21x_destoryMessageBuffer(rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    delete msg;
}

int API_FUNC rf21x_getMessage(rf21x_Device device, rf21x_Message message)
{
    AbstractRF21x *rf = (AbstractRF21x*)device;
    RF21xMessage *msg = (RF21xMessage*)message;
    if (rf->getMessage(*msg))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int API_FUNC rf21x_getMessageType(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->messageType;
}

int API_FUNC rf21x_getQuizType(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->quizType;
}

int API_FUNC rf21x_getQuizNumber(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->quizNumber;
}

int API_FUNC rf21x_getTimeStamp(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->TimeStamp;
}

int API_FUNC rf21x_getKeypadId(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->keypadId;
}

const char* API_FUNC rf21x_getData(const rf21x_Message message)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    return msg->data.c_str();
}

void API_FUNC rf21x_getRawData(const rf21x_Message message, unsigned char **dataPtr, int *dataLength)
{
    RF21xMessage *msg = (RF21xMessage*)message;
    *dataPtr = msg->rawDataBuffer;
    *dataLength = msg->rawDataLength;
}


