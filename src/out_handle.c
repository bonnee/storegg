#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define PATH "../cfg/out_config"
#define N 8

int pins[N];
int out_id;

void sighandle_int(int sig)
{
	printf("out_handle...");
	if (-1 == clear_queue(out_id))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sighandle_int);

	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	pinfile = fopen(PATH, "r");

	if (NULL == pinfile)
	{
		printf("No config\n");
		return -1;
	}

	for (int i = 0; i < N; i++)
	{
		getline(&line, (size_t *)&len, pinfile);
		pins[i] = atoi(line);
	}
	fclose(pinfile);

	for (int i = 0; i < N; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			char str[2];
			sprintf(str, "%d", pins[i]);

			char *args[] = {"./out", str, NULL};
			execvp(args[0], args);
		}
	}

	out_id = create_id(0);
	int handler_id = create_id(2);
	//printf("msgid: %d handleid: %d", msgid, handler_id);

	/*for (int i = 0; i < N; i++)
	{
		values.state[i] = 0;
	}*/
	message msg;

	while (1)
	{
		swbuffer values;

		receive(handler_id, &values, sizeof(values), 2);

		//printf("%d", pins[1]);

		//msg.pin = 8;
		//msg.state = values;
		//printf("out_handle: %d\n", out_id);
		fflush(stdout);

		//printf("senddemmerda %d\n", send(out_id, &msg, sizeof(msg)));

		for (int i = 0; i < N; i++)
		{
			//printf("%d ", values.state[i]);
			msg.pin = pins[i]; // Convert pin to phisical number
			msg.state = values.state[i];

			send(out_id, &msg, sizeof(msg));
		}
		printf("\n");
	}

	wait(NULL);
	return 0;
}