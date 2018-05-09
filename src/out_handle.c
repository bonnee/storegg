#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define PATH "../cfg/out_config"
#define N 8

int pins[N];
swbuffer values;

int main(int argc, char *argv[])
{
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

	message msg;

	int msgid = create_id(0);
	int handler_id = create_id(2);

	/*for (int i = 0; i < N; i++)
	{
		values.state[i] = 0;
	}*/

	while (1)
	{
		receive(handler_id, &values, 2);

		for (int i = 0; i < N; i++)
		{
			printf("%d", values.state[i]);
			msg.pin = pins[i]; // Convert pin to phisical number
			msg.state = values.state[i];
			send(msgid, &msg);
		}
		printf("\n");
	}

	wait(NULL);
	return 0;
}