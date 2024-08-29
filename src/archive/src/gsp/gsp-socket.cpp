#include "gsp-socket.h"

#include <stdio.h>
#include <stdlib.h>

#include "tinythread.h"

/*!
  * \brief Thread for gsp implementation.
  */
static void gsp_thread(void *arg)
{
    GspSocket *gsp = (GspSocket*)arg;

    unsigned char buf[1000];
    while (1)
    {

        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));
        if (gsp->_mutexFinished.try_lock())
        {
            gsp->_mutexFinished.unlock();
            break;
        }

        //printf("Thread \n");
        if (gsp->_mutexEvent.try_lock())
        {
            //printf("Thread lock\n");
            //printf("1. Thread Read\n");
            int r = gsp->readData(buf, sizeof(buf));
//            for (int i = 0; i < r; ++i)
//            {
//                printf("data %d: %X\n", i, buf[i]);
//            }
            if (r > 0)
            {
                gsp->_dataRecvEvent(buf, r);
            }
            //printf("2. Thread Read\n");
            gsp->_mutexEvent.unlock();
        }
    }

    //printf("Thread Return\n");
    return;
}

static void split(const std::string &str, std::vector<std::string> &args)
{
    args.clear();
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < str.length(); ++i)
    {
        //printf("i - %c\n", str[i]);
        if (str[i] != '+')
        {
            for (j = i; j <= str.length(); ++j)
            {
                //printf("j - %c\n", str[j]);
                if (j == str.length() || str[j] == '+')
                {
                    args.push_back(str.substr(i, j - i));
                    i = j;
                    break;
                }
            }
        }
    }
    return;
}


GspSocket::GspSocket()
{
    _socket.Initialize();
    _thread = NULL;
    _timeout = 0;
}

GspSocket::~GspSocket()
{
    close();
}

bool GspSocket::open(const std::string path, int baud)
{
    close();

    int t = 0;

    std::vector<std::string> args;
    split(path, args);
    if (args.size() < 2)
    {
        return false;
    }
    std::string device = args[1];
    std::string address = "";
    int portNumber = 9000;
    int sep = args[0].find(":");
    if (sep > 0)
    {
        address = args[0].substr(0, sep);
        portNumber = atoi(path.substr(sep + 1, path.length() - sep - 1).c_str());
    }
    else if (sep < 0)
    {
        address = args[0];
    }
    else
    {
        return false;
    }
    char cmd[300];
    sprintf(cmd, "open %s %d", device.c_str(), baud);

    _socket.Initialize();
    _socket.SetNonblocking();
    if (_socket.Open((const uint8 *)address.c_str(), portNumber) == false)
    {
        goto failsafe;
    }
    _socket.SetNonblocking();
    _socket.SetSendTimeout(0, 1000 * 100);
    _socket.SetReceiveTimeout(0, 1000 * 10);
    _socket.Send((const uint8 *)cmd, strlen(cmd) + 1);

    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(200));
    t = _socket.Receive(2);
    if (t != 2 || memcmp(_socket.GetData(), "OK", 2) != 0)
    {
        goto failsafe;
    }

    _mutexFinished.lock();
    _thread = new tthread::thread(gsp_thread, this);
    if (_thread == NULL)
    {
        goto failsafe;
    }

    return true;
failsafe:
    close();
    return false;
}

bool GspSocket::close()
{
    if (_thread != NULL)
    {
        _mutexFinished.try_lock();
        _mutexFinished.unlock();
        _thread->join();
        delete _thread;
        _thread = NULL;
    }

    _socket.Close();

    return true;
}

bool GspSocket::isOpened()
{
    return (_thread != NULL);
}

int GspSocket::readData(void *data, int length)
{
    _socket.Select(4, 0);
    int s = 0;
    int t = -1;
    while (1)
    {
        int r = _socket.Receive(length - s);
        if (r >= 0)
        {
            memcpy((char*)data + s, _socket.GetData(), r);
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

int GspSocket::writeData(void *data, int length)
{
    return _socket.Send((const uint8 *)data, length);
}

void GspSocket::flush()
{
    _socket.Receive(1000);
    return;
}

void GspSocket::setTimeout(int ms)
{
    _timeout = ms;
    _socket.SetReceiveTimeout(0, ms * 1000);
}

int GspSocket::readDataNoWaiting(void *data, int length)
{
    _socket.Select(4, 0);
    int s = 0;
    while (1)
    {
        int r = _socket.Receive(length - s);
        if (r >= 0)
        {
            memcpy((char*)data + s, _socket.GetData(), r);
            s += r;
        }
        else
        {
            break;
        }

        if (s >= length)
        {
            break;
        }
    }
    return s;
}
