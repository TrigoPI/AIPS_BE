#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_SERVER 100
#define MAX_CLIENT 100
#define CLOSE -1

typedef int SERVER;
typedef int CLIENT;
typedef int SOCKET;
typedef int BUFFER;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

typedef struct 
{
    int size;
    char data[];
} Buffer;

typedef struct 
{
    int running;
    SOCKET sock;
    SOCKADDR_IN sin;
} ServerData;

typedef struct 
{
    SOCKET sock;
    SOCKADDR_IN sin;
} ClientData;

ServerData* allServer[MAX_SERVER];
ServerData* currentServer;

ClientData* allClient[MAX_CLIENT];
ClientData* currentClient;

CLIENT currentClientID;
SERVER currentServerID;

void cho7_init();
void cho7_useServer(SERVER server);
void cho7_useClient(CLIENT client);
void cho7_closeServer();
void cho7_closeClient();
void cho7_clientData(CLIENT client, struct hostent* hostinfo, int port, int family, int type);
void cho7_serverData(SERVER server, int port, int family, int type, int protocole);

int cho7_serverShouldClose();
int cho7_onConnect();

int cho7_recvFrom(char buffer[], int size);
int cho7_sendTo(char buffer[], int size);

SERVER cho7_createServer();
CLIENT cho7_createClient();