#include "gsp-serial-windows.h"

#include <stdio.h>
#include <queue>
#include <windows.h>

#include "tinythread.h"
#include "Serial/SerialEx.h"


GspSerial::GspSerial()
{
    _timeout = 10;
}

GspSerial::~GspSerial()
{
    close();
}

bool GspSerial::open(const std::string path, int baud)
{
    close();

    std::string p = "\\\\.\\" + path;
    if (Open(p.c_str(), 128, 128) != ERROR_SUCCESS)
    {
        goto failsafe;
    }

    Setup(baud, CSerial::EData8, CSerial::EParNone, CSerial::EStop1);
    SetupHandshaking(CSerial::EHandshakeOff);
    SetMask(CSerial::EEventRecv | CSerial::EEventRcvEv);
    SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
    StartListener();

    return true;

failsafe:
    close();
    return false;
}

bool GspSerial::close()
{
    if (IsOpen())
    {
        StopListener();
        Close();
    }
    return true;
}

bool GspSerial::isOpened()
{
    return this->IsOpen();
}

int GspSerial::readData(void *data, int length)
{
    DWORD s = 0;
    int t = -1;
    while (1)
    {
        DWORD r = 0;
        Read((char*)data + s, length - s, &r);
        if (r > 0)
        {
            s += r;
            if (s >= length)
            {
                break;
            }
        }
        else
        {
            ++t;
            if (t >= _timeout)
            {
                break;
            }
            tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));
        }
        //printf("readData: %d\n", r);
    }
    return s;
}

int GspSerial::writeData(void *data, int length)
{
    DWORD s = 0;
    int t = -1;
    while (1)
    {
        DWORD r = 0;
        Write((char*)data + s, length, &r);
        if (r > 0)
        {
            s += r;
            if (s >= length)
            {
                break;
            }
        }
        else
        {
            ++t;
            if (t >= _timeout)
            {
                break;
            }
            tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));
        }
    }
    return s;
}

void GspSerial::flush()
{
    DWORD got = 0;
    char buf[GSP_MAX_DATA_SIZE];
    Read(buf, sizeof(buf), &got, NULL, 10);
    return;
}

void GspSerial::setTimeout(int ms)
{
    this->_timeout = ms;
}

void GspSerial::OnEvent(CSerial::EEvent eEvent, CSerial::EError eError)
{
    if (_mutexEvent.try_lock())
    {
        _timeout = 20;
        unsigned char buf[1000];
        int r = readDataNoWaiting(buf, sizeof(buf));
        if (r > 0)
        {
            _dataRecvEvent(buf, r);
        }
        _mutexEvent.unlock();
    }
}

int GspSerial::readDataNoWaiting(void *data, int length)
{
    DWORD s = 0;

    while (1)
    {
        DWORD got = 0;
        Read((char*)data + s, length - s, &got);

        if (got > 0)
        {
            s += got;
        }
        else
        {
            break;
        }

        if (s >= length)
        {
            break;
        }
        //printf("readData: %d\n", got);
    }

    return s;
}
