#include "SocketClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

SocketClient::SocketClient()
{
    _client_sockfd = 0;
    memset(&_remote_addr, 0, sizeof(_remote_addr));
    memset(_buf, 0, sizeof(_buf));
}

SocketClient::~SocketClient()
{
     disconnect();
}

bool SocketClient::connect(const char *addr, int port)
{
    if (_client_sockfd > 0)
    {
        return true;
    }
    _remote_addr.sin_family = AF_INET;
    _remote_addr.sin_addr.s_addr = inet_addr(addr);
    _remote_addr.sin_port = htons(port);

    _client_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(_client_sockfd < 0)
    {
        perror("socket");
        return false;
    }
    int nZero = 0;
    setsockopt(_client_sockfd, SOL_SOCKET, SO_SNDBUF, &nZero, sizeof(nZero));
    setsockopt(_client_sockfd, SOL_SOCKET, SO_RCVBUF, &nZero, sizeof(nZero));
    int nNetTimeout = 1000;
    setsockopt(_client_sockfd, SOL_SOCKET, SO_SNDTIMEO, &nNetTimeout, sizeof(nNetTimeout));
    setsockopt(_client_sockfd, SOL_SOCKET, SO_RCVTIMEO, &nNetTimeout, sizeof(nNetTimeout));

    if(::connect(_client_sockfd, (struct sockaddr*)&_remote_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("connect");
        return false;
    }
    return true;
}

bool SocketClient::disconnect()
{
    if (_client_sockfd == 0)
    {
        return true;
    }
    close(_client_sockfd);
    _client_sockfd = 0;
}

bool SocketClient::isConnected()
{
    if (_client_sockfd > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SocketClient::send(const uint8_t *buf, int len)
{
    return ::send(_client_sockfd, buf, len, 0);
}

int SocketClient::recv(uint8_t *buf, int len)
{
    return ::recv(_client_sockfd, buf, len, 0);
}
