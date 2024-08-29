/*!
  * \file gsp-wrap.h
  * Cpp wrap of general serial port.
  */

#ifndef __GSP_WRAP_H__
#define __GSP_WRAP_H__

#include "gsp-api.h"
#include <string>
#include <vector>

namespace gsp
{

class GspDevice
{
public:
    /*!
     * \brief Get HID device serial number.
     *  User can use serial number as address which like "hid://0007800D" to select a hid device to be opened.
     * \return serial number in an vector.
     */
    static std::vector<std::string> getHidSerialNumber();

    /*!
     * \brief Open device with a specified port url.
     * \param port Serial port name.
     * Example of serialPort:
     * - serial port device:
     *   - "serial://COM1" (Windows)
     *   - "serial://COM2" (Windows)
     *   - "serial:///dev/ttyUSB0" (Linux)
     *   - "serial:///dev/ttyUSB1" (Linux)
     * - serial port with HID device:
     *   - "hid://"
     *   - "hid://0007800D"
     * - network device:
     *   - "tcp://192.168.1.1:9000+serial://COM1"
     *   - "tcp://192.168.2.2:9000+hid://"
     * \return Success or fail.
     */
    bool open(const char *port, int baud);

    /*!
     * \brief Close device.
     * \return Success or fail.
     */
    bool close();

    /*!
     * \brief isOpened Check device is opened.
     * \return
     */
    bool isOpened();

    /*!
     * \brief readData Read data from device.
     * \param data Data vector.
     * \param length Data lenght need to be read.
     * \return Actual data lenght has been read.
     */
    int readData(std::vector<unsigned char> &data, int length);

    /*!
     * \brief readData Write data to device.
     * \param data Data vector.
     * \return Actual data lenght has been write.
     */
    int writeData(std::vector<unsigned char> &data);

    /*!
     * \brief flushReciveFIFO Flush the buffer of data receiving.
     */
    void flushReciveFIFO();

private:
    gsp_Device _gsp;
};

}

#endif
