/*!
  * \brief A simple wrap of socket client.
  */

#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include <stdio.h>
#include <netinet/in.h>

class SocketClient
{
public:
    SocketClient();
    virtual ~SocketClient();

    bool connect(const char *addr, int port);
    bool disconnect();
    bool isConnected();

    int send(const uint8_t *buf, int len);
    int recv(uint8_t *buf, int len);

private:
#ifdef WIN32
#else
    int _client_sockfd;
    struct sockaddr_in _remote_addr;
    char _buf[BUFSIZ];
#endif
};

#endif
