#include "rf21x.h"
#include <stdlib.h>
#include <string.h>
#include "gsp.h"

AbstractRF21x::AbstractRF21x()
{
}

AbstractRF21x::~AbstractRF21x()
{
}

RF21xMessage::RF21xMessage()
{
    messageType = MT_Unknow;
    quizType = QT_Unknow;
    keypadId = 0;

    memset(rawDataBuffer, 0, RF21X_MAX_RAW_DATA_LENGTH);
    rawDataLength = 0;
}

void RF21xMessage::saveRawData(unsigned char *buffer, int length)
{
    if (length > RF21X_MAX_RAW_DATA_LENGTH)
    {
        length = RF21X_MAX_RAW_DATA_LENGTH;
    }
    memcpy(rawDataBuffer, buffer, length);
    rawDataLength = length;
}

RF21xQuizParams::RF21xQuizParams()
{
    param1 = -1;
    param2 = -1;
	newQuiz = 1;
}

bool getHidSerialNumber(int index, std::string &str)
{
    return Gsp::getHidSerialNumber(index, str);
}

#ifdef _WIN32
bool getHidTempPath(int index, std::string &str)
{
    return Gsp::getHidTempPath(index, str);
}

bool checkHidCable(const std::string str)
{
    return Gsp::checkHidCable(str);
}
#endif
