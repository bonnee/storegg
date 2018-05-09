#include "io.h"

int pin;

int main(int argc, char *argv[])
{

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
		//printf("%d = %d\n", cur, msg.state);

		if (msg.state != cur)
		{
			msg.state = cur;
			send(msgid, &msg);
		}
		sleep(0);
	}

	if (-1 == pinUnexport(pin))
		return (4);

	return (0);
}