/*!
  * \brief Used for OpenWRT.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "tinythread.h"
#include "gsp.h"

void split(const std::string &str, std::vector<std::string> &args)
{
    args.clear();
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < str.length(); ++i)
    {
        //printf("i - %c\n", str[i]);
        if (!isspace(str[i]))
        {
            for (j = i; j <= str.length(); ++j)
            {
                //printf("j - %c\n", str[j]);
                if (j == str.length() || isspace(str[j]))
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

tthread::thread *thread = NULL;
tthread::mutex mutexFinished;

int serverSocket = -1;
struct sockaddr_in serverAddress;

int clientSocket = -1;
struct sockaddr_in clientAddress;
socklen_t clientAddressSize;

int clientNewSocket = -1;
struct sockaddr_in clientNewAddress;
socklen_t clientNewAddressSize;


static void espThread(void *arg)
{
    Gsp gsp;
    unsigned char buf[1000];

    int n = 0;
    n = 10; // buffer length
    setsockopt(clientSocket, SOL_SOCKET, SO_SNDBUF, &n, sizeof(n));
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
    n = 10; // timeout
    setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, &n, sizeof(n));
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &n, sizeof(n));

    tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));

    while (1)
    {
        if (mutexFinished.try_lock())
        {
            mutexFinished.unlock();
            break;
        }
        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));

        if (gsp.isOpened() == false)
        {
            int len = recv(clientSocket, buf, sizeof(buf), 0);
            if (len <= 0)
            {
                continue;
            }
            std::string cmd;
            std::vector<std::string> args;
            cmd.append((const char *)buf, len);
            printf("Get Command: \"%s\"\n", cmd.c_str());
            split(cmd, args);
            if (args.size() != 3 || args[0].compare("open") != 0)
            {
                continue;
            }
            if (gsp.open(args[1], atoi(args[2].c_str())))
            {
                gsp.disableRecvEvent();
                gsp.setTimeout(10);
                send(clientSocket, "OK", 2, 0);
            }
        }

        //printf("waiting socket\n");

        int retry = 0;
        int r = recv(clientSocket, buf, sizeof(buf), 0);
        int s = 0;
        while (r > 0 && s < r && ++retry < 1000)
        {
            int t = gsp.writeData(buf + s, r - s);
            if (t > 0)
            {
                s += t;
            }
            if (s < r)
            {
                tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));
            }
        }
        if (r > 0)
        {
            printf("socket got data: ");
            for (int i = 0; i < r; ++i)
            {
                printf("%02X ", buf[i]);
            }
            printf("\n");
        }

        //printf("waiting serial port\n");

        retry = 0;
        r = gsp.readData(buf, sizeof(buf));
        s = 0;
        while (r > 0 && s < r && ++retry < 1000)
        {
            int t = send(clientSocket, buf + s, r - s, 0);
            if (t > 0)
            {
                s += t;
            }
            if (s < r)
            {
                tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));
            }
            //printf("send: %d\n", s);
        }
        if (r > 0)
        {
            printf("serial port got data: ");
            for (int i = 0; i < r; ++i)
            {
                printf("%02X ", buf[i]);
            }
            printf("\n");
        }
    }
    close(clientSocket);
    clientSocket = 0;

    //printf("Thread Return.\n");
    return;
}


int main(int argc, char **argv)
{
    printf("#####################################\n");
    printf("# Server application of esp.\n");
    printf("# Usage: esp-server [port]\n");
    printf("#####################################\n\n");
    int serverPort = 9000;
    if (argc == 2)
    {
        serverPort = atoi(argv[1]);
    }
    printf("Starting esp socket server.\n");

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        perror("Socket initializing error");
        return 1;
    }

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(struct sockaddr)) < 0)
    {
        perror("Socket bind error");
        return 1;
    }

    printf("Listening port %d.\n", serverPort);
    if (listen(serverSocket, 5) < 0)
    {
        perror("Fail to listen port.\n");
        return 1;
    }

    while (1)
    {
        printf("Waiting for new connection.\n");
        clientNewAddressSize = sizeof(struct sockaddr_in);
        clientNewSocket = accept(serverSocket, (struct sockaddr*)&clientNewAddress, &clientNewAddressSize);
        signal(SIGPIPE, SIG_IGN);
        int flags;
        flags = fcntl(clientNewSocket, F_GETFL,0);
        fcntl(clientNewSocket, F_SETFL, flags | O_NONBLOCK);
        if (clientNewSocket < 0)
        {
            perror("Fail to accept.\n");
            return 1;
        }

        if (thread != NULL)
        {
            mutexFinished.unlock();
            thread->join();
            delete thread;
            thread = NULL;
            printf("Old thread closed.\n");
        }

        clientSocket = clientNewSocket;
        clientAddressSize = clientNewAddressSize;
        memcpy(&clientAddress, &clientNewAddress, sizeof(struct sockaddr_in));

        mutexFinished.try_lock();
        thread = new tthread::thread(espThread, NULL);
        if (thread == NULL)
        {
            printf("Fail to create new thread.\n");
        }
    }

    return 0;
}
