#include "rf21x-wrap.h"
#include <stdio.h>

std::vector<std::string> rf21x::getHidSerialNumber()
{
    std::vector<std::string> snVector;
    int index = 0;
    while (1)
    {
        char buf[100];
        if (rf21x_getHidSerialNumber(index, buf, sizeof(buf)))
        {
            snVector.push_back(buf);
        }
        else
        {
            break;
        }
        ++index;
    }
    return snVector;
}

#ifdef _WIN32
std::vector<std::string> rf21x::getHidTempPath()
{
    std::vector<std::string> snVector;
    int index = 0;
    while (1)
    {
        char buf[100];
        if (rf21x_getHidTempPath(index, buf, sizeof(buf)))
        {
            snVector.push_back(buf);
        }
        else
        {
            break;
        }
        ++index;
    }
    return snVector;
}

bool rf21x::checkHidCable(const std::string tempPath)
{
    if (rf21x_checkHidCable(tempPath.c_str()) == 0)
    {
        return false;
    }
    return true;
}
#endif

rf21x::RF21xDevice::RF21xDevice()
{
    _device = NULL;
    _message = rf21x_createMessageBuffer();
}

rf21x::RF21xDevice::~RF21xDevice()
{
    close();
    rf21x_destoryMessageBuffer(_message);
}

bool rf21x::RF21xDevice::open(int type, const char *port, int minId, int maxId)
{
    close();

    _device = rf21x_createDevice(type);
    if (_device == NULL)
    {
        return false;
    }

    if (rf21x_open(_device, port, minId, maxId) == false)
    {
        close();
        return false;
    }
    return true;
}

bool rf21x::RF21xDevice::close()
{
    if (_device != NULL)
    {
        rf21x_destoryDevice(_device);
        _device = NULL;
    }
    return true;
}

bool rf21x::RF21xDevice::startQuiz(int type, int param1, int param2, int newQuiz )
{
    if (_device == NULL)
    {
        return false;
    }
    return (rf21x_startQuiz(_device, type, param1, param2, newQuiz) != 0);
}

bool rf21x::RF21xDevice::startDirectly(int type, void *buffer)
{
    if (_device == NULL)
    {
        return false;
    }
    return (rf21x_startQuizDirectly(_device, type, buffer) != 0);
}

bool rf21x::RF21xDevice::stopQuiz()
{
    if (_device == NULL)
    {
        return false;
    }
    return (rf21x_stopQuiz(_device) != 0);
}

bool rf21x::RF21xDevice::setKeypadId(int id)
{
    if (_device == NULL)
    {
        return false;
    }
    return (rf21x_setKeypadId(_device, id) != 0);
}

unsigned char rf21x::RF21xDevice::GetQuizResult(int id,int QuizNo)
{
	if (_device == NULL)
    {
        return 0;
    }
    return (rf21x_GetQuizResult(_device, id, QuizNo));
}

void rf21x::RF21xDevice::ClearQuizResult()
{
	if (_device!=NULL)
	{
		rf21x_ClearQuizResult(_device);
	}
}

bool rf21x::RF21xDevice::readDeviceSerialNumber(int deviceType, const char *devicePort, std::string &sn, bool isDefault)
{
    if (_device == NULL)
    {
        _device = rf21x_createDevice(deviceType);
    }
    char buf[10];
    int isOk = rf21x_readDeviceSerialNumber(_device, devicePort, buf, sizeof(buf), isDefault);
    if (isOk)
    {
        sn = "";
        sn += buf;
    }
    return (isOk != 0);
}

bool rf21x::RF21xDevice::writeDeviceSerialNumber(int deviceType, const char *devicePort, const std::string sn)
{
    if (_device == NULL)
    {
        _device = rf21x_createDevice(deviceType);
    }
    return (rf21x_writeDeviceSerialNumber(_device, devicePort, sn.c_str()) != 0);
}

bool rf21x::RF21xDevice::resetDeviceSerialNumber(int deviceType, const char *devicePort)
{
    if (_device == NULL)
    {
        _device = rf21x_createDevice(deviceType);
    }
    return (rf21x_resetDeviceSerialNumber(_device, devicePort) != 0);
}

bool rf21x::RF21xDevice::checkDeviceSerialNumber(int deviceType, const char *devicePort, const std::string sn)
{
    if (_device == NULL)
    {
        _device = rf21x_createDevice(deviceType);
    }
    return (rf21x_checkDeviceSerialNumber(_device, devicePort, sn.c_str()) != 0);
}

bool rf21x::RF21xDevice::getMessage(rf21x::RF21xMessage &message)
{
    if (_device == NULL)
    {
        return false;
    }
    if (rf21x_getMessage(_device, _message) == 0)
    {
        return false;
    }
    message.messageType = rf21x_getMessageType(_message);
    message.quizType = rf21x_getQuizType(_message);
    message.quizNumber = rf21x_getQuizNumber(_message);
    message.keypadId = rf21x_getKeypadId(_message);
    message.data = rf21x_getData(_message);
	message.timeStamp=rf21x_getTimeStamp(_message);

    unsigned char *rawDataPtr;
    int rawDataLength;

    message.rawData.clear();
    rf21x_getRawData(_message, &rawDataPtr, &rawDataLength);
    for (int i = 0; i < rawDataLength; ++i)
    {
        message.rawData.push_back(rawDataPtr[i]);
    }
    return true;
}

