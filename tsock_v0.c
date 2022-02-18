#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "cho7.h"
#include "tsock.h"

int main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	
	int bufferSize = -1;
	int nb_message = -1;
	int source = -1; 
	int puit = -1;

	int udp = -1;
	int tcp = -1;

	int port = -1;

	char host[100];

	// p : puit
	// u : udp 
	// t : tcp
	// n : nb messages

	while ((c = getopt(argc, argv, "ptusn:l:")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) 
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			else
			{
				puit = 1;
			}

			break;
		
		case 's':
			if (puit == 1) 
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			else 
			{
				source = 1;
			}

			break;

		case 't':
			if (udp == 1)
			{
				printf("erreur\n");
				exit(1);
			}
			tcp  = 1;
			break;

		case 'u':
			if (tcp == 1)
			{	
				printf("erreur\n");
				exit(1);
			}

			udp = 1;

			break;

		case 'l' :
			bufferSize = atoi(optarg);
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (puit == -1 && source == -1) 
	{
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (nb_message == -1) 
	{
		if (source == 1)
		{
			nb_message = 10;
		}
	} 

	if (bufferSize == -1)
	{
		bufferSize = 30;
	}

	if (source == 1)
	{	
		port = atoi(argv[argc-1]);
		strcpy(host, argv[argc-2]);

		if (tcp == 1)
		{	
			Client client = tsock_createTCPClient(host, port, bufferSize);
			
			printf("Client : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=tcp, dest=%s\n", bufferSize, port, nb_message, host);
		
			tsock_sendTCPMessage(client, nb_message);
		}

		if (udp == 1)
		{
			Client client = tsock_createUDPClient(host, port, bufferSize);

			printf("Client : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=udp, dest=%s\n", bufferSize, port, nb_message, host);
			
			tsock_sendUPDMessage(client, nb_message);
		}
	}

	if (puit == 1)
	{
		port = atoi(argv[argc-1]);

		if (tcp == 1)
		{
			Server server = tsock_createTCPServer(port, bufferSize);

			if (nb_message <= 0)
			{
				printf("Server : g_mesg_lu=%d, listenning=0.0.0.0, port=%d, nb_receptions=infini, TP=tcp\n", bufferSize, port);
			}
			else 
			{
				printf("Server : g_mesg_lu=%d, listenning=0.0.0.0, port=%d, nb_receptions=%d, TP=tcp\n", bufferSize, port, nb_message);
			}

			tsock_runTCPServer(server, nb_message);
		}

		if (udp == 1)
		{	

			Server server = tsock_createUDPServer(port, bufferSize);

			if (nb_message <= 0)
			{
				printf("Server : g_mesg_lu=%d, listenning=0.0.0.0, port=%d, nb_receptions=infini, TP=udp\n", bufferSize, port);
			}
			else 
			{
				printf("Server : g_mesg_lu=%d, listenning=0.0.0.0, port=%d, nb_receptions=%d, TP=udp\n", bufferSize, port, nb_message);
			}
			
			tsock_runUPDServer(server, nb_message);
		}
	}

	return 0;
}

