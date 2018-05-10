#include "io.h"

int pin;

int main(int argc, char *argv[])
{
	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	if (-1 == pinExport(pin))
		return (1);

	sleep(1); // brutto

	if (-1 == pinDirection(pin, 1))
		return (2);

	int status = LOW;

	int msgid = create_id(0);
	//printf("msgid: %d\n", msgid);

	message msg;
	while (1)
	{
		receive(msgid, &msg, sizeof(msg), pin);
		printf("%d received %d: %d\n", pin, msg.pin, msg.state);
		pinWrite(pin, msg.state);

		int rc;
		struct msqid_ds buf;
		rc = msgctl(msgid, IPC_STAT, &buf);
		printf("Messages: %d\n", buf.msg_qnum);

		//sleep(1);
	}

	if (-1 == pinUnexport(pin))
		return (4);

	return (0);
}