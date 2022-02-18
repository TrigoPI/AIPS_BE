#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys/socket.h>

#include "cho7.h"

CHO7 cho7_createServer()
{
    CHO7 id = currentServerID;
    Cho7* server = (Cho7*)malloc(sizeof(Cho7));
    
    server->running = 0;
    allServer[id] = server;

    ++currentServerID;

    return id;
}

CHO7 cho7_createClient()
{
    CHO7 id = currentClientID;
    Cho7* client = (Cho7*)malloc(sizeof(Cho7));
    
    allClient[id] = client;

    ++currentClientID;

    return id;
}

BUFFER cho7_createBuffer(int size)
{
    BUFFER id = currentBufferID;
    Buffer* buffer = (Buffer*)malloc(sizeof(Buffer));

    buffer->data = (char*)malloc(sizeof(char)*size); 
    buffer->size = size;

    allBuffer[id] = buffer;

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

int cho7_send(BUFFER buffer)
{
    assert(currentClient != NULL);
    assert(currentClient->type != UDP);

    Buffer* buf = allBuffer[buffer];

    int result = send(currentClient->sock, buf->data, buf->size, 0);

    return result;
}

int cho7_recv(BUFFER buffer)
{
    assert(currentClient != NULL);
    assert(currentClient->type != UDP);

    Buffer* buf = allBuffer[buffer];

    int result = recv(currentClient->sock, buf->data, buf->size, 0);

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

int cho7_getBufferSize(BUFFER buffer)
{
    return allBuffer[buffer]->size;
}

const char* cho7_getBufferData(BUFFER buffer)
{
    return allBuffer[buffer]->data;
}

void cho7_listen(int max)
{
    assert(currentServer != NULL);
    assert(currentServer->type != UDP);

    listen(currentServer->sock, max);
}

void cho7_accept()
{
    assert(currentServer != NULL);
    assert(currentClient != NULL);

    int size = sizeof(currentClient->sin);

    currentClient->sock = accept(currentServer->sock, (SOCKADDR*)&currentClient->sin, &size);
}

void cho7_connect()
{
    assert(currentClient != NULL);
    assert(currentClient->type != UDP);

    connect(currentClient->sock, (SOCKADDR*)&currentClient->sin, sizeof(currentClient->sin));
}

void cho7_closeServer()
{
    assert(currentServer != NULL);

    currentServer->running = 0;
    close(currentServer->sock);
}

void cho7_closeClient()
{
    assert(currentClient != NULL);

    close(currentClient->sock);
}

void cho7_useServer(CHO7 server)
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

void cho7_useClient(CHO7 client)
{
    if (currentClient == NULL)
    {
        currentClient = allClient[client];
        currentClient->running = 1;
    }
    else 
    {
        if (client == CLOSE)
        {
            currentClient->running = 0;
            currentClient = NULL;
        }
        else 
        {
            currentClient = allClient[client];
            currentClient->running = 1;
        }
    }
}

void cho7_clientData(CHO7 client, struct hostent* hostinfo, int port, int family, int type)
{
    Cho7* clt = allClient[client];
    
    clt->type = type;
    clt->sock = socket(family, type, 0);
    clt->sin.sin_addr = *(IN_ADDR*)hostinfo->h_addr;
    clt->sin.sin_port = htons(port);
    clt->sin.sin_family = family;

    bind(clt->sock, (SOCKADDR*)&clt->sin, sizeof(clt->sin));
}

void cho7_serverData(CHO7 server, int port, int family, int type, int protocole)
{
    Cho7* srv = allServer[server];

    srv->type = type;
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