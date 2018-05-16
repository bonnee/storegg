#include "io.h"
#include "msg_queue.h"
#include <signal.h>

int pin;

//when the process is closed, it unExports pins
void sighandle_int(int sig)
{
	if (pinUnexport(pin) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	//replaces the standard sigint handler
	signal(SIGINT, sighandle_int);

	//Error, it means that there is no pin number passed as parameter
	if (argc <= 1)
		return 3;

	//converts the argument of the function into an int number
	pin = atoi(argv[1]);

	//Enable GPIO pins
	if (-1 == pinExport(pin))
		return (1);

	//Set GPIO directions
	if (-1 == pinDirection(pin, 1))
		return (2);

	int status = LOW;

	//creates the output queue to communicate with the out_handle
	int msgid = create_id(0);

	message msg;
	while (1)
	{
		//receives the message of the correspondent pin from the output queue
		receive(msgid, &msg, sizeof(msg), pin);
		//writes the value received in the right output pin
		pinWrite(pin, msg.state);
	}

	return (0);
}