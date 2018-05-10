#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define N 8

int msgid;

void sighandle_int(int sig)
{
	printf("Exiting...");
	if (-1 == clear_queue(msgid))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sighandle_int);
	for (int i = 0; i < 2; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			char *args[] = {i ? "./out_handle" : "./in_handle", NULL};
			execvp(args[0], args);
		}
	}
	/*values.type = 1;
	for (int i = 0; i < N; i++)
	{
		values.state[i] = 0;
	}*/
	msgid = create_id(2);

	while (1)
	{
		swbuffer values;

		//printf("%d", sizeof(values));
		receive(msgid, &values, sizeof(values), 1);

		swbuffer sendvalues;
		sendvalues.type = 2;

		printf("handler: ");
		for (int i = 0; i < N; i++)
		{
			sendvalues.state[i] = values.state[i];

			printf("%d ", sendvalues.state[i]);
		}
		printf("\n");

		send(msgid, &sendvalues, sizeof(sendvalues));
	}
	wait(NULL);
}