#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define PATH "../cfg/in_config"
#define N 8

// 0-5 eggs
// 6-7 storage
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

	for (int i = 0; i < N; i++) // Read config
	{
		getline(&line, (size_t *)&len, pinfile);
		pins[i] = atoi(line);
	}
	fclose(pinfile);

	for (int i = 0; i < N; i++) // Create children
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			char str[2];
			sprintf(str, "%d", pins[i]);

			char *args[] = {"./in", str, NULL};
			execvp(args[0], args);
		}
	}

	for (int i = 0; i < N; i++) // Init values
	{
		values.state[i] = 0;
	}

	message msg;

	int hw_id = create_id(1);
	int handler_id = create_id(2);

	values.type = 1;

	while (1)
	{
		receive(hw_id, &msg, 0);

		int index;

		for (index = 0; index < N && pins[index] != msg.pin; index++)
			;

		values.state[index] = msg.state;

		printf("in_handle: ");
		for (int i = 0; i < N; i++)
		{
			printf("%d", values.state[i]);
		}
		printf("\n");

		send(handler_id, &values);

		//printf("receive %d: %d\n", msg.pin, msg.state);
	}

	wait(NULL);
	return 0;
}