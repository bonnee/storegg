#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"
#include "logic.h"

int N;
int msgid;

// deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("Exiting...");
	if (clear_queue(msgid) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	// Attach exit handler
	signal(SIGINT, sighandle_int);

	// Parameter check
	if (argc != 4)
	{
		fprintf(stderr, "Parameter error.\nUsage: %s IN_CONFIG OUT_CONFIG N_EGGS\n", argv[0]);
		return 1;
	}

	//gets the number of eggs passed as a parameter
	N = atoi(argv[3]);

	//Check if the number of eggs choosen by the user can be managed by the program
	if (N > MAX_NUM_EGGS)
	{
		fprintf(stderr, "Too much eggs!\n");
		return 2;
	}

	// as there will be two processes for the in_handle and the out_handle we need two different
	// buffers to pass as parameters of the execvp function
	char *arg_in[] = {"./in_handle", argv[2], argv[3], NULL};
	char *arg_out[] = {"./out_handle", argv[1], argv[3], NULL};

	for (int i = 0; i < 2; i++)
	{
		// forks the input and output handlers
		pid_t pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "Failed to fork the processes\n");
			return 3;
		}
		if (pid == 0)
		// depending on the value of i we execute the two different handlers
		// i=0 means in_handle; i=1 out_handle
		{
			if ((i ? execvp(arg_out[0], arg_out) : execvp(arg_in[0], arg_in)) == -1)
			{
				return 4;
			};
		}
	}

	// creates the queue to communicate with the two handlers
	msgid = create_id(2);

	// Blink leds to signal program start
	int value = 0;
	swbuffer initVal;
	// type=2 to be received only by out_handle
	initVal.type = 2;

	for (int times = 0; times < 2; times++)
	{
		value = !value;

		for (int pin = 0; pin < N + 2; pin++)
		{
			initVal.state[pin] = value;
		}

		send(msgid, &initVal, sizeof(initVal));
		sleep(1);
	}
	// All ready

	while (1)
	{
		swbuffer values;

		// receives the input array from the in_handle
		receive(msgid, &values, sizeof(values), 1);

		swbuffer sendvalues;
		// type=2 to be received only by out_handle
		sendvalues.type = 2;

		// calculates the output array
		calc_output(values.state, sendvalues.state);

		// send the output array to the out_handle
		send(msgid, &sendvalues, sizeof(sendvalues));
	}

	//the parent process waits till the child processes are done
	wait(NULL);
	return 0;
}