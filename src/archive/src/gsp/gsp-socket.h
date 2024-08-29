#ifndef __GENERAL_SERIAL_PORT_SOCKET_H__
#define __GENERAL_SERIAL_PORT_SOCKET_H__

#include "gsp.h"

#include <stdio.h>
#include <string.h>
#include <vector>

#include "tinythread.h"
#include "ActiveSocket.h"

static void gsp_thread(void *arg);

class GspSocket : public AbstractGsp
{
    friend void gsp_thread(void *arg);

public:
    explicit GspSocket();
    virtual ~GspSocket();

    virtual bool open(const std::string path, int baud);
    virtual bool close();
    virtual bool isOpened();

    virtual int readData(void *data, int length);
    virtual int writeData(void *data, int length);

    virtual void flush();
    virtual void setTimeout(int ms);

private:
    int readDataNoWaiting(void *data, int length);

private:
    CActiveSocket _socket;
    int _timeout;

    tthread::thread *_thread;
    //tthread::mutex _mutexDevice;
    tthread::mutex _mutexFinished;
};

#endif
