#include "io.h"
#include "msg_queue.h"
#include <signal.h>

int pin;

//when the process is closed, it unExports pins
void sighandle_int(int sig)
{
	if (pinUnexport(pin) == -1)
	{
		exit(EXIT_FAILURE);
	}
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
	//printf("Pin: %d\n", pin);

	//Enable GPIO pins
	if (-1 == pinExport(pin))
		return (1);

	//Set GPIO directions
	if (-1 == pinDirection(pin, 0))
		return (2);

	message msg;
	//set the message type and the value (reading the value of the pin)
	msg.pin = pin;
	msg.state = 0;

	//creates the input queue to communicate with the in_handle
	int msgid = create_id(1);

	while (1)
	{
		//keeps reading the values from in_pins
		int cur = pinRead(pin);

		if (msg.state != cur)
		{
			//printf("pin %d: %d\n",pin,msg.state);
			//if the value changes then it creates a message and sends it
			msg.state = cur;
			send(msgid, &msg, sizeof(msg));
		}
		usleep(100000); // It's need not to kill CPU
	}

	return (0);
}