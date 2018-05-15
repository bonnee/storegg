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
		return -1;

	//converts the argument of the function into an int number
	pin = atoi(argv[1]);
	//printf("Pin: %d\n", pin);

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
		//printf("%d received %d: %d\n", pin, msg.pin, msg.state);
		//writes the value received in the right output pin
		pinWrite(pin, msg.state);
	}

	//stessa cosa di in.c qua
	if (-1 == pinUnexport(pin))
		return (4);

	return (0);
}