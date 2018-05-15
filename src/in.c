#include "io.h"
#include "msg_queue.h"
#include <signal.h>

int pin;

void sighandle_int(int sig)
{
	//printf("I'm unexporting pins\n");
	if (pinUnexport(pin) == -1)
	{
		printf("Failed\n");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sighandle_int);

	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	/*
	 * Enable GPIO pins
	 */
	if (-1 == pinExport(pin))
		return (1);
	/*
	 * Set GPIO directions
	 */
	if (-1 == pinDirection(pin, 0))
		return (2);

	message msg;
	msg.pin = pin;
	msg.state = pinRead(pin);

	int msgid = create_id(1);

	while (1)
	{
		int cur = pinRead(pin);

		if (msg.state != cur)
		{

			//printf("%d = %d\n", cur, msg.state);
			msg.state = cur;
			send(msgid, &msg, sizeof(msg));
		}
		usleep(100000); // Don't kill CPU please
	}

	if (-1 == pinUnexport(pin))
		return (4);

	return (0);
}