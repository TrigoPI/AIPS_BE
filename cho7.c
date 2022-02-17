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
    server->running = 0;
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

BUFFER cho7_createBuffer(int size)
{
    BUFFER id = currentBufferID;
    allBuffer[id] = (Buffer*)malloc(sizeof(Buffer));
    allBuffer[id]->data = (char*)malloc(sizeof(char)*size); 

    ++currentBufferID;

    return id;
}

int cho7_serverShouldClose()
{
    assert(currentServer != NULL);

    return currentServer->running;
}

int cho7_recvFrom(BUFFER buffer)
{
    assert(currentClient != NULL);
    assert(currentServer != NULL);

    Buffer* buf = allBuffer[buffer];
    int fromSize = sizeof(currentClient->sin);
    int result = recvfrom(currentServer->sock, buf->data, buf->size, 0, (SOCKADDR*)&currentClient->sin, &fromSize);

    return result;
}

int cho7_sendTo(BUFFER buffer)
{
    assert(currentClient != NULL);

    Buffer* buf = allBuffer[buffer];
    int toSize = sizeof(currentClient->sin);
    int result = sendto(currentClient->sock, buf->data, buf->size, 0, (SOCKADDR*)&currentClient->sin, toSize);

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

void cho7_clientData(CLIENT client, struct hostent* hostinfo, int port, int family, int type)
{
    ClientData* clt = allClient[client];
    
    clt->sock = socket(family, type, 0);
    clt->sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    clt->sin.sin_port = htons(port);
    clt->sin.sin_family = family;

    bind(clt->sock, (SOCKADDR*)&clt->sin, sizeof(clt->sin));
}

void cho7_serverData(SERVER server, int port, int family, int type, int protocole)
{
    ServerData* srv = allServer[server];

    srv->sin.sin_addr.s_addr = INADDR_ANY;
    srv->sin.sin_family = family;
    srv->sin.sin_port = htons(port);
    srv->sock = socket(family, type, protocole);

    bind(srv->sock, (SOCKADDR*)&srv->sin, sizeof(srv->sin));
}

void cho7_bufferData(BUFFER buffer, char data[])
{
    Buffer* buf = allBuffer[buffer];
    strcpy(buf->data, data);
}