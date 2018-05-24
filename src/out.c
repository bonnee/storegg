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
	if (argc <= 1)
	{
		fprintf(stdout, "No parameter provided.\nUsage: %s PIN\n", argv[0]);
		return 3;
	}

	pin = atoi(argv[1]);

	// Enable GPIO pin
	if (-1 == pinExport(pin))
		return (1);

	// Set GPIO direction
	if (-1 == pinDirection(pin, 1))
		return (2);

	// Message queue for handler process
	int msgid = create_id(0);

	message msg;
	while (1)
	{
		// Receives the message of the correspondent pin from the output queue
		receive(msgid, &msg, sizeof(msg), pin);
		printf("%d: %d\n", pin, msg.state);
		pinWrite(pin, msg.state);
	}

	return (0);
}