#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

int N;
int *pins;
int out_id;

// deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("out_handle...");
	if (-1 == clear_queue(out_id))
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
		fprintf(stderr, "Parameter error.\nUsage: %s OUT_CONFIG N_LEDS\n", argv[0]);
		return 1;
	}

	N = atoi(argv[2]) + 2; //N bit output + 2 bit for storage
	pins = (int *)malloc(N * sizeof(int));

	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	//tries to open out_config
	pinfile = fopen(argv[1], "r");
	if (NULL == pinfile)
	{
		fprintf(stderr, "No config\n");
		return 2;
	}

	// reads the numbers of output pins from out_config
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
			char *args[] = {"./out", str, NULL};
			if (execvp(args[0], args) == -1)
			{
				fprintf(stderr, "Error executing process for pin %d\n", pins[i]);
				return 5;
			}
		}
	}

	// Message queue for hw processes
	out_id = create_id(0);

	// Message queue for handler
	int handler_id = create_id(2);

	message msg;

	while (1)
	{
		swbuffer values;
		// receives the message from the handler queue containing the output array
		receive(handler_id, &values, sizeof(values), 2);

		for (int i = 0; i < N; i++)
		{
			msg.pin = pins[i];
			msg.state = values.state[i];
			send(out_id, &msg, sizeof(msg));
		}
	}

	// the parent process waits till the child processes are done
	wait(NULL);
	return 0;
}