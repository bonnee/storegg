#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define PATH "../cfg/in_config"
int N = 8;

int *pins;
swbuffer values;
int in_id;

//deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("in_handle...");
	if (-1 == clear_queue(in_id))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	//replaces the standard sigint handler
	signal(SIGINT, sighandle_int);

	//controls if the n° of parameters is correct
    if(argc != 2){
        printf("Invalid number of parameters");
        return 1;
    }

    N = atoi(argv[1]) + 2; //N bit output + 2 bit for storage
	pins = (int*) malloc(N * sizeof(int));

	//reads the numbers of input pins from a config file
	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	//tries to open the in_config file
	pinfile = fopen(PATH, "r");
	if (NULL == pinfile)
	{
		printf("No config\n");
		return 2;
	}

	for (int i = 0; i < N; i++) // Read in_config
	{
		getline(&line, (size_t *)&len, pinfile);
		//saves the numbers of pins in an array
		pins[i] = atoi(line);
	}
	//tries to close the file where it's reading the pins' values
	if(fclose(pinfile) == -1)
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
			char *args[] = {"./in", str, NULL};
			if(execvp(args[0], args)== -1)
        	{
            	return 5;
        	};
		}
	}

	for (int i = 0; i < N; i++) // Initializes values
	{
		values.state[i] = 0;
	}

	//creates the input queue to communicate with the singles in_pins processes
	in_id = create_id(1);
	//creates the handler queue to communicate with the handler
	int handler_id = create_id(2);

	values.type = 1;

	printf("in_handle ready.\n");

	while (1)
	{
		message msg;
		//receives the message from the input queue containing the pins' values
		receive(in_id, &msg, sizeof(msg), 0);

		int index;

		//parses the pin number position into an index of a new array
		for (index = 0; index < N && pins[index] != msg.pin; index++);

		//sets the input array
		values.state[index] = msg.state;

		//sends the input array to the handler through the handler queue
		send(handler_id, &values, sizeof(values));

	}

	//the parent process waits till the child processes are done
	wait(NULL);
	return 0;
}