#include "rf21x-base.h"
#include "rf217.h"
#include "rf218.h"
#include "rf219.h"
#include "rf215.h"
#include "rf228.h"

#ifdef WIN32

static LARGE_INTEGER globalCount;


void tic()
{
    QueryPerformanceCounter(&globalCount);
    printf("From Tic...\n");
}

double toc()
{
    LARGE_INTEGER freq;
    LARGE_INTEGER count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    double t = (count.QuadPart - globalCount.QuadPart);
    t = t / freq.QuadPart;
    printf("To Toc: %f ms...\n", t * 1000);
    return t;
}

#endif

const char* RF21x::_teacherMessagesString[12] = {
    "up",
    "ok",
    "power",
    "result",
    "report",
    "start/pause",
    "stop",
    "down",
    "mode",
    "question",
    "f1",
    "f2"
};

AbstractRF21x *createRF21x(RF21xDeviceType type)
{
    if (type == DT_RF217)
    {
        return (new RF217());
    }
    else if (type == DT_RF218)
    {
        return (new RF218());
    }
    else if (type == DT_RF219)
    {
        return (new RF219());
    }
    else if (type == DT_RF215)
	{
		return (new RF215());
	}
    else if (type == DT_RF228)
    {
        return (new RF228());
    }
	else
    {
        return NULL;
    }
}

void destoryRF21x(AbstractRF21x *rf21x)
{
    delete rf21x;
}

bool RF21x::getMessage(RF21xMessage &message)
{
    _mutexMessage.lock();
    bool isOk = false;
    if (_messageFifo.empty() == false)
    {
        message = _messageFifo.front();
        _messageFifo.pop_front();
        isOk = true;
    }
    _mutexMessage.unlock();
    return isOk;
}

void RF21x::_executeMessageEvent(const RF21xMessage &message)
{
    _mutexMessage.lock();
    _messageFifo.push_back(message);
    _mutexMessage.unlock();
}


