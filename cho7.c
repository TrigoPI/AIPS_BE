#include <stdlib.h>
#include <assert.h>

#include "cho7.h"

void cho7_init()
{
    currentClientID = 0;
    currentServerID = 0;

    currentServer = NULL;
    currentClient = NULL; 
}

SERVER cho7_createServer()
{
    SERVER id = currentServerID;
    ServerData* server = (ServerData*)malloc(sizeof(ServerData));

    allServer[id] = server;

    ++currentServerID;

    return id;
}

CLIENT cho7_createClient()
{
    CLIENT id = currentClientID;

    allClient[id] = (ClientData*)malloc(sizeof(ClientData));

    ++currentClientID;

    return id;
}

int cho7_serverShouldClose()
{
    assert(currentServer != NULL);

    return currentServer->running;
}

int cho7_recvFrom(char buffer[], int size)
{
    assert(currentClient != NULL);
    assert(currentServer != NULL);

    int fromSize = sizeof(currentClient->sin);

    int result = recvfrom(currentServer->sock, buffer, size, 0, (SOCKADDR*)&currentClient->sin, &fromSize);

    return result;
}

void cho7_closeServer()
{
    assert(currentServer != NULL);

    currentServer->running = 0;
    close(currentServer->sock);
    currentServer = NULL;
}

void cho7_useServer(SERVER server)
{
    if (currentServer == NULL)
    {
        currentServer = allServer[server];
        currentServer->running = 1;
    }
    else 
    {
        if (server == CLOSE)
        {
            currentServer->running = 0;
            currentServer = NULL;
        }
        else 
        {
            currentServer = allServer[server];
            currentServer->running = 1;
        }
    }
}

void cho7_useClient(CLIENT client)
{
    if (currentClient == NULL)
    {
        currentClient = allClient[client];
    }
    else 
    {
        if (client == CLOSE)
        {
            currentClient = NULL;
        }
        else 
        {
            currentClient = allClient[client];
        }
    }
}

void cho7_setClientInfo(struct hostent* hostinfo, int port, int family)
{
    assert(currentClient != NULL);

    currentClient->sin.sin_addr = *(IN_ADDR*)hostinfo->h_addr;
    currentClient->sin.sin_port = htons(port);
    currentClient->sin.sin_family = family;
} 