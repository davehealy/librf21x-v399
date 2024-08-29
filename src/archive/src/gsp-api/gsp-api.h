/*!
  * \file gsp-api.h
  * C API of general serial port.
  */

#ifndef __GSP_API_H__
#define __GSP_API_H__

// for SWIG
#ifdef _MSC_VER
    #define API_FUNC __stdcall
    #ifdef LIBGSP_EXPORTS
        #define LIBGSP_API __declspec(dllexport)
    #else
        #define LIBGSP_API __declspec(dllimport)
    #endif
#else
    #define API_FUNC
    #define LIBGSP_API
#endif


class Gsp;

typedef void* gsp_Device;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Get HID device serial number.
 *  User can use serial number as address whick like "hid://0007800D" to select a hid device to be opened.
 * \param index Index number of device, start from 0.
 * \param buffer Serial number got from device.
 * \param maxLength max length.
 * \return success or fail(0).
 */
LIBGSP_API int API_FUNC gsp_getHidSerialNumber(int index, char *buffer, int maxLength);

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
 * \return Device handle or NULL.
 */
LIBGSP_API gsp_Device API_FUNC gsp_open(const char *port, int baud);

/*!
 * \brief Close device.
 * \return Success or fail(0).
 */
LIBGSP_API int API_FUNC gsp_close(gsp_Device device);

LIBGSP_API int API_FUNC gsp_readData(gsp_Device device, void *data, int length);

LIBGSP_API int API_FUNC gsp_writeData(gsp_Device device, void *data, int length);

LIBGSP_API void API_FUNC gsp_flushReciveFIFO(gsp_Device device);


#ifdef __cplusplus
}
#endif

#endif
