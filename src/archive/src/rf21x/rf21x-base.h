#ifndef __RF21X_BASE_H__
#define __RF21X_BASE_H__

#include <string>
#include <map>
#include <deque>

#include "rf21x.h"
#include "tinythread.h"

class RF21x : public AbstractRF21x
{
public:
    virtual bool getMessage(RF21xMessage &message);

protected:
    static const char* _teacherMessagesString[12];
    void _executeMessageEvent(const RF21xMessage &message);

private:
    tthread::mutex _mutexMessage;
    std::deque<RF21xMessage> _messageFifo;
};

void tic();
double toc();


#endif
