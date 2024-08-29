#ifndef __GENERAL_SERIAL_PORT_SERIAL_H__
#define __GENERAL_SERIAL_PORT_SERIAL_H__

#include "gsp.h"

#include <stdio.h>
#include <string.h>

#include "tinythread.h"
#include "cssl/cssl.h"


static void esp_thread(void *arg);

class GspSerial : public AbstractGsp
{
    friend void esp_thread(void *arg);

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
    int _read(void *data, int length);
    int _write(void *data, int length);

private:
    cssl_t *_device;
    int _timeout;

    tthread::thread *_thread;
    tthread::mutex _mutexDevice;
    tthread::mutex _mutexFinished;
};

#endif
