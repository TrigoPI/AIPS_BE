#include "tsock.h"
#include "cho7.h"
#include "stdio.h"
#include <netdb.h>

Server tsock_createUDPServer(int port, int bufferSize)
{
    CHO7 serverID = cho7_createServer();
    CHO7 clientID = cho7_createClient();
    BUFFER bufferID = cho7_createBuffer(bufferSize);

    cho7_serverData(serverID, port, AF_INET, SOCK_DGRAM, 0);

    return (Server){ serverID, clientID, bufferID };
}

Client tsock_createUDPClient(const char* hostname, int port, int bufferSize)
{
    CHO7 clientID = cho7_createClient();
    BUFFER bufferID = cho7_createBuffer(bufferSize);
    HOSTENT hostinfo = gethostbyname(hostname);

    cho7_clientData(clientID, hostinfo, port, AF_INET, SOCK_DGRAM);

    return (Client){ clientID, bufferID };
}

void tsock_runUPDServer(Server server, int max)
{
    int n = 0;

    cho7_useServer(server.serverID);
    cho7_useClient(server.clientID);

    while (cho7_serverShouldClose()) 
    {
        int result = cho7_recvFrom(server.bufferID);

        if (result)
        {
            printf("Server : Reception n°%d (%d) [----%d%s]\n", n, cho7_getBufferSize(server.bufferID), n, cho7_getBufferData(server.bufferID));
            ++n;
        }
        else
        {
            printf("erreur\n");
        }

        if (max > 0)
        {
            if (n > max)
            {
                cho7_closeClient();
                cho7_closeServer();           
            }
        }
    }

    cho7_useClient(CLOSE);
    cho7_useServer(CLOSE);
}

void tsock_sendUPDMessage(Client client, int nbMessage)
{
    int bufferSize = cho7_getBufferSize(client.bufferID);
    cho7_useClient(client.clientID);    

    for (int i = 0; i < nbMessage; ++i)
    {
        char message[bufferSize];

        for (int j = 0; j < bufferSize - 1; ++j)
        {
            message[j] = messages[i];
        }

        message[bufferSize - 1] = '\0';

        printf("Client : Envoi n°%d (%d) [----%d%s]\n", i, bufferSize, i, message);

        cho7_bufferData(client.bufferID, message);
        cho7_sendTo(client.bufferID);
    }
}