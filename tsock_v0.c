/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

#include <string.h>

#include "cho7.h"

void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	
	int udp = -1;
	int tpc = -1;

	int port = -1;

	char host[100];

	// p : puit
	// u : udp 
	// t : tcp
	// n : nb messages

	while ((c = getopt(argc, argv, "pusn:")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;
		
		case 'u':
			udp = 1;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1)
	{
		printf("on est dans le source\n");

		port = atoi(argv[argc-1]);
		strcpy(host, argv[argc-2]);

		struct hostent* hostinfo = gethostbyname(host);

		if (hostinfo == NULL)
		{
			printf("error");
			exit(EXIT_FAILURE);
		}

		CLIENT clt = cho7_createClient();

		cho7_clientData(clt, hostinfo, port, AF_INET, SOCK_DGRAM);
		cho7_useClient(clt);

		printf("send\n");

		int n = cho7_sendTo("test", 5);

		if (n)
		{
			printf("super\n");
		}
		else 
		{
			printf("erreur\n");
		}
	}
	else
	{
		port = atoi(argv[argc-1]);

		printf("listenning on 0.0.0.0 %d\n", port);

		if (udp)
		{
			char buffer[1024];
			
			SERVER srv = cho7_createServer();
			CLIENT clt = cho7_createClient();

			cho7_serverData(srv, port, AF_INET, SOCK_DGRAM, 0);

			cho7_useServer(srv);
			cho7_useClient(clt);

			while (cho7_serverShouldClose())
			{
				int n = cho7_recvFrom(buffer, 1023);

				if (n)
				{
					buffer[n] = '\0';
					printf("%s\n", buffer);
				}
				else 
				{
					printf("ERREUR");
				}
			}
		}
	}

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
}

