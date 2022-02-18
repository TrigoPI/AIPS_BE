#ifndef TSOCK_H
#define TSOCK_H

#include "cho7.h"

#define SIZE 27

typedef struct
{
    CHO7 serverID;
    CHO7 clientID;
    BUFFER bufferID;
} Server;

typedef struct 
{
    CHO7 clientID;
    BUFFER bufferID;
} Client;

static char messages[SIZE] = "abcdefghijklmnopqrstuvw";

Server tsock_createUDPServer(int port, int bufferSize);
Server tsock_createTCPServer(int port, int bufferSize);

Client tsock_createUDPClient(const char* hostname, int port, int bufferSize);
Client tsock_createTCPClient(const char* hostname, int port, int bufferSize);

void tsock_runUPDServer(Server server, int max);
void tsock_runTCPServer(Server server, int max);

void tsock_sendUPDMessage(Client client, int nbMessage);
void tsock_sendTCPMessage(Client client, int nbMessage);

#endif