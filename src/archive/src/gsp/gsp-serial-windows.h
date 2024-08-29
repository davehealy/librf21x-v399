#ifndef __GENERAL_SERIAL_PORT_SERIAL_WINDOWS_H__
#define __GENERAL_SERIAL_PORT_SERIAL_WINDOWS_H__

#include "gsp.h"

#include <stdio.h>
#include <queue>

#include "tinythread.h"
#include "Serial\SerialEx.h"


class GspSerial : public AbstractGsp, private CSerialEx
{

public:
    explicit GspSerial();
    virtual ~GspSerial();

    virtual bool open(const std::string path, int baud);
    virtual bool close();
    virtual bool isOpened();

    virtual int readData(void *data, int length);
    virtual int writeData(void *data, int length);

    virtual void flush();
    virtual void setTimeout(int ms);

private:
    virtual void OnEvent(CSerial::EEvent eEvent, CSerial::EError eError);
    int readDataNoWaiting(void *data, int length);

private:
    int _timeout;
};

#endif
