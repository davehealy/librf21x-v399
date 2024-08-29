#include "gsp-serial-unix.h"

/*!
  * \brief Thread for esp implementation.
  */
static void esp_thread(void *arg)
{
    GspSerial *esp = (GspSerial*)arg;

    unsigned char buf[1000];
    while (1)
    {
        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));
        if (esp->_mutexFinished.try_lock())
        {
            esp->_mutexFinished.unlock();
            break;
        }

        //printf("Thread \n");
        if (esp->_mutexEvent.try_lock())
        {
            //printf("Thread lock\n");
            //printf("1. Thread Read\n");
            int r = esp->readData(buf, sizeof(buf));
//            for (int i = 0; i < r; ++i)
//            {
//                printf("data %d: %X\n", i, buf[i]);
//            }
            if (r > 0)
            {
                esp->_dataRecvEvent(buf, r);
            }
            //printf("2. Thread Read\n");
            esp->_mutexEvent.unlock();
        }
    }

    //printf("Thread Return\n");
    return;
}

GspSerial::GspSerial()
{
    _device = NULL;
    _thread = NULL;
    _timeout = 10;
    cssl_start();
}

GspSerial::~GspSerial()
{
    close();
    cssl_stop();
}

bool GspSerial::open(const std::string path, int baud)
{
    close();

    _device = cssl_open(path.c_str(), NULL, 0, baud, 8, 0, 1);
    if (_device == NULL)
    {
        goto failsafe;
    }

    cssl_setflowcontrol(_device, 0, 0);
    cssl_settimeout(_device, 10);

    _mutexFinished.lock();
    _thread = new tthread::thread(esp_thread, this);
    if (_thread == NULL)
    {
        goto failsafe;
    }

    return true;
failsafe:
    close();
    return false;
}

bool GspSerial::close()
{
    if (_thread != NULL)
    {
        _mutexFinished.try_lock();
        _mutexFinished.unlock();
        _thread->join();
        delete _thread;
        _thread = NULL;
    }

    if (_device != NULL)
    {
        cssl_close(_device);
        _device = NULL;
    }
}

bool GspSerial::isOpened()
{
    return (_device != NULL);
}

int GspSerial::readData(void *data, int length)
{
    int s = 0;
    for (int t = -10; t < _timeout; t += 10)
    {
        int r = cssl_getdata(_device, (uint8_t*)data + s, length - s);
        if (r > 0)
        {
            s += r;
        }
        if (s >= length)
        {
            break;
        }
        else
        {
            tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));
        }
        //printf("readData: %d\n", got);
    }
    return s;
}

int GspSerial::writeData(void *data, int length)
{
    cssl_putdata(_device, (uint8_t*)data, length);
    cssl_drain(_device);
    return length;
}

void GspSerial::flush()
{
    unsigned char buf[100];
    cssl_getdata(_device, buf, sizeof(buf));
    return;
}

void GspSerial::setTimeout(int ms)
{
    _timeout = ms;
}
