#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define N 8

int values[N];

int main(int argc, char *argv[])
{
	for (int i = 0; i < 2; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			char *args[] = {i ? "./out_handle" : "./in_handle", NULL};
			execvp(args[0], args);
		}
	}

	int msgid = create_id(2);
	swbuffer values;

	while (1)
	{
		receive(msgid, &values, 1);

		printf("handler: ");
		for (int i = 0; i < N; i++)
		{
			printf("%d", values.state[i]);
		}
		printf("\n");

		values.type = 2;
		send(msgid, &values);
	}
	wait(NULL);
}