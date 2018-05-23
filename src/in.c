#include "io.h"
#include "msg_queue.h"
#include <signal.h>

int pin;

// Unexport pins before exiting
void sighandle_int(int sig)
{
	if (pinUnexport(pin) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	// Attach exit handler
	signal(SIGINT, sighandle_int);

	//Error, it means that there is no pin number passed as parameter
	if (argc != 1)
	{
		fprintf(stdout, "No parameter provided.\nUsage: %s PIN\n", argv[0]);
		return 3;
	}

	pin = atoi(argv[1]);

	//Enable GPIO pin
	if (-1 == pinExport(pin))
		return (1);

	//Set GPIO direction
	if (-1 == pinDirection(pin, 0))
		return (2);

	message msg;
	msg.pin = pin;
	msg.state = 0;

	// creates the input queue to communicate with the in_handle
	int msgid = create_id(1);

	while (1)
	{
		int cur = pinRead(pin);

		if (msg.state != cur)
		{
			//if the value changes then it creates a message and sends it
			msg.state = cur;
			send(msgid, &msg, sizeof(msg));
		}
		usleep(100000); // Sleep a while to avoid CPU overload
	}

	return (0);
}