#include "gsp-api.h"
#include "gsp.h"

#include <string.h>
#include <deque>

class GspHelper : public GspRecvEvent
{
public:
    Gsp gsp;
    // FIFO, front in, back out.
    std::deque<unsigned char> buffer;
    tthread::mutex mutex;

    virtual void dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length)
    {
        mutex.lock();
        for (int i = 0; i < length; ++i)
        {
            unsigned char c = ((unsigned char*)data)[i];
            if (buffer.size() > 100*1000)
            {
                buffer.pop_back();
            }
            buffer.push_front(c);
        }
        mutex.unlock();
    }
};

int API_FUNC gsp_getHidSerialNumber(int index, char *buffer, int maxLength)
{
    std::string str;
    if (Gsp::getHidSerialNumber(index, str) && (maxLength > str.length()))
    {
        strcpy(buffer, str.c_str());
        return 1;
    }
    return 0;
}

gsp_Device API_FUNC gsp_open(const char *port, int baud)
{
    GspHelper *gsph = new GspHelper;
    if (gsph->gsp.open(port, baud))
    {
        gsph->gsp.setRecvEvent(gsph);
        gsph->gsp.setTimeout(0);
        gsph->gsp.enableRecvEvent();
        return gsph;
    }

    delete gsph;
    return NULL;
}

int API_FUNC gsp_close(gsp_Device device)
{
    GspHelper *gsph = (GspHelper*)device;
    if (gsph == NULL)
    {
        return 0;
    }
    delete gsph;
    return 1;
}

int API_FUNC gsp_readData(gsp_Device device, void *data, int length)
{
    GspHelper *gsph = (GspHelper*)device;
    if (gsph == NULL)
    {
        return 0;
    }
    int i = 0;
    gsph->mutex.lock();
    for (i = 0; i < length; ++i)
    {
        if (gsph->buffer.empty())
        {
            break;
        }
        ((unsigned char*)data)[i] = gsph->buffer.back();
        gsph->buffer.pop_back();
    }
    gsph->mutex.unlock();
    return i;
}

int API_FUNC gsp_writeData(gsp_Device device, void *data, int length)
{
    GspHelper *gsph = (GspHelper*)device;
    if (gsph == NULL)
    {
        return 0;
    }
    return gsph->gsp.writeData(data, length);
}


void API_FUNC gsp_flushReciveFIFO(gsp_Device device)
{
    GspHelper *gsph = (GspHelper*)device;
    if (gsph == NULL)
    {
        return;
    }

    gsph->gsp.flush();

    gsph->mutex.lock();
    gsph->buffer.clear();
    gsph->mutex.unlock();

    return;
}
