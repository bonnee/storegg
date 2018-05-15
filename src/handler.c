#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"
#include "logic.h"

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

    if(argc != 2){
        printf("Invalid number of parameters");
        return 1;
    }

    for (int i = 0; i < 2; i++)
	{
		//fork the input and output handlers
		pid_t pid = fork();
		if (pid == 0)
		{
			char *args[] = {i ? "./out_handle" : "./in_handle", argv[1], NULL};
			execvp(args[0], args);
		}
	}

	//creates the queue to communicate with the two handlers
	msgid = create_id(2);

	while (1)
	{
		swbuffer values;

		//receives the input array
		receive(msgid, &values, sizeof(values), 1);

		swbuffer sendvalues;
		//type=2 to be received only by out_handle
		sendvalues.type = 2;

		printf("handler: ");
		for (int i = 0; i < N; i++)
		{
			//sendvalues.state[i] = values.state[i];
			printf("%d ", values.state[i]);
		}
		printf("\n");

		//calculates the output array
		calc_output(values.state,sendvalues.state);

		printf("output array: ");
		for (int i = 0; i < N; i++)
		{
			printf("%d ", sendvalues.state[i]);
		}
		printf("\n");

		// send the output array to the output handler
		send(msgid, &sendvalues, sizeof(sendvalues));
	}
	wait(NULL);
}