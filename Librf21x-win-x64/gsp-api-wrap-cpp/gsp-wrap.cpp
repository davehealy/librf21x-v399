#include "gsp-wrap.h"

#include <stdlib.h>

namespace gsp
{

std::vector<std::string> GspDevice::getHidSerialNumber()
{
    std::vector<std::string> snVector;
    int index = 0;
    while (1)
    {
        char buf[100];
        if (gsp_getHidSerialNumber(index, buf, sizeof(buf)))
        {
            snVector.push_back(buf);
        }
        else
        {
            break;
        }
        ++index;
    }
    return snVector;
}


bool GspDevice::open(const char *port, int baud)
{
    close();
    _gsp = gsp_open(port, baud);
    return isOpened();
}

bool GspDevice::close()
{
    if (isOpened())
    {
        gsp_close(_gsp);
    }
    _gsp = NULL;
    return true;
}


bool GspDevice::isOpened()
{
    return (_gsp != NULL);
}

int GspDevice::readData(std::vector<unsigned char> &data, int length)
{
    if (isOpened() == false)
    {
        return 0;
    }
    data.clear();
    char *buffer = new char[length];
    int r = gsp_readData(_gsp, buffer, length);
    for (int i = 0; i < r; ++i)
    {
        data.push_back(buffer[i]);
    }
    delete[] buffer;
    return r;
}

int GspDevice::writeData(std::vector<unsigned char> &data)
{
    if (isOpened() == false)
    {
        return 0;
    }
	unsigned char *d = (unsigned char*)malloc(data.size());
	for (size_t i = 0; i < data.size(); ++i)
	{
		d[i] = data[i];
	}
    int isOk = gsp_writeData(_gsp, d, data.size());
	free(d);
	return isOk;
}

void GspDevice::flushReciveFIFO()
{
    if (isOpened() == false)
    {
        return;
    }
    gsp_flushReciveFIFO(_gsp);
}

}
