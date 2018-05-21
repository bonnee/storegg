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

//deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("out_handle...");
	if (-1 == clear_queue(out_id))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	//replaces the standard sigint handler
	signal(SIGINT, sighandle_int);

	//controls if the n° of parameters is correct
	if (argc != 3)
	{
		printf("Invalid number of parameters");
		return 1;
	}

	N = atoi(argv[2]) + 2; //N bit output + 2 bit for storage
	pins = (int *)malloc(N * sizeof(int));

	//reads the numbers of output pins from a config file
	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	//tries to open the out_config file
	pinfile = fopen(argv[1], "r");
	if (NULL == pinfile)
	{
		printf("No config\n");
		return 2;
	}

	for (int i = 0; i < N; i++) // Read out_config
	{
		getline(&line, (size_t *)&len, pinfile);
		//saves the numbers of pins in an array
		pins[i] = atoi(line);
	}
	//tries to close the file where it's reading the pins' values
	if (fclose(pinfile) == -1)
	{
		printf("Failed to close file");
		return 3;
	}

	for (int i = 0; i < N; i++) // Create children for each pin
	{
		pid_t pid = fork();

		if (pid < 0)
		{
			printf("Failed to fork the processes");
			return 4;
		}
		if (pid == 0)
		{
			char str[2];
			sprintf(str, "%d", pins[i]);
			//passes the number of pin as input in order to know which pin it's referring to
			char *args[] = {"./out", str, NULL};
			if (execvp(args[0], args) == -1)
			{
				return 5;
			}
		}
	}

	//creates the output queue to communicate with the singles out_pins processes
	out_id = create_id(0);
	//creates the handler queue to communicate with the handler
	int handler_id = create_id(2);

	message msg;

	printf("out_handle ready.\n");
	while (1)
	{
		swbuffer values;
		//receives the message from the handler queue containing the output array
		receive(handler_id, &values, sizeof(values), 2);

		printf("out: ");
		for (int i = 0; i < N; i++)
		{
			printf("%d ", values.state[i]);
			msg.pin = pins[i];			 // Converts pin to phisical number, it sets the type of the message equal to the pin number
			msg.state = values.state[i]; //sets the pin value reading the output array
			//sends the message to the singles out processes through the output queue
			send(out_id, &msg, sizeof(msg));
		}
		printf("\n");
	}

	//the parent process waits till the child processes are done
	wait(NULL);
	return 0;
}