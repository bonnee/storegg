#include "io.h"
#include "msg_queue.h"
#include <signal.h>

int pin;

void sighandle_int(int sig)
{
	//printf("exiting/n");
	if (pinUnexport(pin) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

	signal(SIGINT, sighandle_int);

	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	if (-1 == pinExport(pin))
		return (1);

	//sleep(1);
	//usleep(pin*20000); // brutto

	if (-1 == pinDirection(pin, 1))
		return (2);

	int status = LOW;

	int msgid = create_id(0);
	//printf("msgid: %d\n", msgid);

	message msg;
	while (1)
	{
		receive(msgid, &msg, sizeof(msg), pin);
		//printf("%d received %d: %d\n", pin, msg.pin, msg.state);
		pinWrite(pin, msg.state);
	}

	if (-1 == pinUnexport(pin))
		return (4);

	return (0);
}