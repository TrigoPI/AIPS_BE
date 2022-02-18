#ifndef CHO7_H
#define CHO7_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_SERVER 100
#define MAX_CLIENT 100
#define MAX_BUFFER 100
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define CLOSE -1

typedef int CHO7;
typedef int SOCKET;
typedef int BUFFER;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct hostent* HOSTENT;

typedef struct 
{
    int size;
    char* data;
} Buffer;

typedef struct 
{
    int running;
    int type;
    SOCKET sock;
    SOCKADDR_IN sin;
} Cho7;

static Buffer* allBuffer[MAX_BUFFER];
static   Cho7* allClient[MAX_CLIENT];
static   Cho7* allServer[MAX_SERVER];

static Cho7* currentServer = NULL;
static Cho7* currentClient = NULL;

static BUFFER currentBufferID = 0;
static CHO7 currentClientID   = 0;
static CHO7 currentServerID   = 0;


void cho7_closeServer();
void cho7_closeClient();
void cho7_accept();
void cho7_connect();
void cho7_listen(int max);
void cho7_useServer(CHO7 server);
void cho7_useClient(CHO7 client);
void cho7_clientData(CHO7 client, struct hostent* hostinfo, int port, int family, int type);
void cho7_serverData(CHO7 server, int port, int family, int type, int protocole);
void cho7_bufferData(BUFFER buffer, char data[]);

int cho7_serverShouldClose();
int cho7_recvFrom(BUFFER buffer);
int cho7_recv(BUFFER buffer);
int cho7_sendTo(BUFFER buffer);
int cho7_send(BUFFER buffer);
int cho7_getBufferSize(BUFFER buffer);

const char* cho7_getBufferData(BUFFER buffer);

CHO7 cho7_createServer();
CHO7 cho7_createClient();
BUFFER cho7_createBuffer(int size);

#endif