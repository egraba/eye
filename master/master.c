#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 9999
#define MAX_CLIENTS 50



void terminate_master();
void terminate_child();



pid_t pid;
int kill_child;



void
terminate_master()
{
	exit(EXIT_SUCCESS);
}

void
terminate_child()
{
	kill_child = 1;
}



int
main(void)
{
	int s;
	int c;
	struct sockaddr_in sa;
	unsigned int len;
	char data[BUFSIZ];

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket error...");
		exit(EXIT_FAILURE);
	}

	explicit_bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(DEFAULT_PORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(s, (struct sockaddr*) &sa, sizeof(sa)) < 0) {
		perror("Bind error...");
		exit(EXIT_FAILURE);
	}

	listen(s, MAX_CLIENTS);
	printf("Listening...\n");

	for (;;) {
		len = sizeof(sa);

		if ((c = accept(s, (struct sockaddr*) &sa, &len)) < 0) {
			perror("Accept error...");
			exit(EXIT_FAILURE);
		}
		printf("Client accepted!\n");

		pid = fork();

		if (pid < 0) {
			perror("Fork error...");
			close(s);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
			kill_child = 0;
			signal(SIGINT, terminate_child);
			signal(SIGPIPE, terminate_child);
		
			while(!kill_child) {
				int data_len;

				explicit_bzero(data, BUFSIZ);
				data_len = recv(c, data, BUFSIZ, 0);

				if (data_len < 0) {
					perror("Recv error...");
				}
				else if (data_len == 0) {
					break;
				}
				else
					printf("Received:\n%s\n\n", data);
			}
			
			close(c);
		}
		else {
			wait(NULL);
		}

		close(s);
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_SUCCESS);
}
