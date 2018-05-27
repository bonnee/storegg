#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

int N;
int *pins;
swbuffer values;
int in_id;

// deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("in_handle...");
	free(pins);
	if (-1 == clear_queue(in_id))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	// Attach exit handler
	signal(SIGINT, sighandle_int);

	// Parameter check
	if (argc != 3)
	{
		fprintf(stderr, "Parameter error.\nUsage: %s IN_CONFIG N_EGGS\n", argv[0]);
		return 1;
	}

	N = atoi(argv[2]) + 2; // N eggs + 2 bit for storage management
	pins = (int *)malloc(N * sizeof(int));

	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	// tries to open in_config
	pinfile = fopen(argv[1], "r");
	if (NULL == pinfile)
	{
		fprintf(stderr, "No config\n");
		return 2;
	}

	// reads the numbers of input pins
	for (int i = 0; i < N; i++)
	{
		getline(&line, (size_t *)&len, pinfile);
		pins[i] = atoi(line);
	}

	if (fclose(pinfile) == -1)
	{
		fprintf(stderr, "Failed to close file\n");
		return 3;
	}

	for (int i = 0; i < N; i++) // Create children for each pin
	{
		pid_t pid = fork();

		if (pid < 0)
		{
			fprintf(stderr, "Failed to fork the processes\n");
			return 4;
		}
		if (pid == 0)
		{
			char str[2];
			sprintf(str, "%d", pins[i]);

			// Pass hw pin number to child
			char *args[] = {"./in", str, NULL};
			if (execvp(args[0], args) == -1)
			{
				fprintf(stderr, "Error executing process for pin %d\n", pins[i]);
				return 5;
			};
		}
	}

	for (int i = 0; i < N; i++)
	{
		values.state[i] = 0;
	}

	// Message queue for hw processes
	in_id = create_id(1);

	// Message queue for handler
	int handler_id = create_id(2);

	// type = 1 to be received only by the handler
	values.type = 1;

	while (1)
	{
		message msg;
		// receives the message from the input queue containing the pins' new values
		receive(in_id, &msg, sizeof(msg), 0);

		int index;

		// Converts hw pin numbers into logical pin numbers
		for (index = 0; index < N && pins[index] != msg.pin; index++);

		// sets the input array
		values.state[index] = msg.state;

		// sends the input array to the handler
		send(handler_id, &values, sizeof(values));
	}

	// the parent process waits till the child processes are done
	wait(NULL);
	return 0;
}