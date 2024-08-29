/*!
  * \brief Used for OpenWRT.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "PassiveSocket.h"
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

int main(int argc, char **argv)
{
    printf("#####################################\n");
    printf("# Server application of gsp.\n");
    printf("# Usage: gsp-server [port]\n");
    printf("#####################################\n\n");
    int serverPort = 9000;
    if (argc == 2)
    {
        serverPort = atoi(argv[1]);
    }
    printf("Starting gsp socket server.\n");

    CPassiveSocket server;
    server.Initialize();
    server.SetNonblocking();

    printf("Listening port %d.\n", serverPort);
    if (server.Listen((const uint8*)"0.0.0.0", serverPort) == false)
    {
        printf("Fail to listen port %d.\n", serverPort);
        return 0;
    }

    CActiveSocket *client = NULL;
    CActiveSocket *clientNew = NULL;
    Gsp gsp;
    while (1)
    {
        //printf("Waiting for connection.\n");
        clientNew = server.Accept();
        if (clientNew != NULL)
        {
            printf("Got new connection.\n");
            if (client != NULL)
            {
                client->Close();
                delete client;
                client = NULL;
            }
            gsp.close();

            client = clientNew;
            clientNew = NULL;

            client->SetNonblocking();
            client->SetSendTimeout(0, 1000 * 10);
            client->SetReceiveTimeout(0, 1000 * 10);
        }

        if (client != NULL)
        {
            if (gsp.isOpened() == false)
            {
                tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));
                int r = client->Receive(100);
                if (r <= 0)
                {
                    continue;
                }
                std::string cmd;
                std::vector<std::string> args;
                cmd.append((const char *)client->GetData(), r);
                printf("Get Command: \"%s\"\n", cmd.c_str());
                split(cmd, args);
                if (args.size() != 3 || args[0].compare("open") != 0)
                {
                    continue;
                }
                std::string port = args[1];
                int baud = atoi(args[2].c_str());
                if (gsp.open(port, baud) == false)
                {
                    continue;
                }
                gsp.disableRecvEvent();
                gsp.setTimeout(5);
                client->Send((const uint8*)"OK", 2);
                printf("Success to make a gsp connection.\n", cmd.c_str());
            }
            tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1));

            //printf("Begin of send & receive.\n");

            int retry = 0;
            int r = client->Receive(1000);
            int s = 0;
            while (r > 0 && s < r && ++retry < 100)
            {
                int t = gsp.writeData(client->GetData() + s, r - s);
                if (t > 0)
                {
                    s += t;
                }
            }

//            for (int i = 0; i < s; ++i)
//            {
//                printf("Receive: %X.\n", (client->GetData())[i]);
//            }

            unsigned char buf[1000];
            retry = 0;
            r = gsp.readData(buf, sizeof(buf));
            s = 0;
            while (r > 0 && s < r && ++retry < 100)
            {
                int t = client->Send((const uint8*)(buf + s), r - s);
                if (t > 0)
                {
                    s += t;
                }
            }

//            for (int i = 0; i < s; ++i)
//            {
//                printf("Send (%d): %X.\n", i, (client->GetData())[i]);
//            }

            //printf("End of send & receive.\n");
        }
    }

    return 0;
}
