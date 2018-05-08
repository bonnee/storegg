#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

typedef struct hw_buffer
{
	long pin; // pin fisico
	int state;
} message;

int create_id(int in)
{
	char *file = "../cfg/out_config";

	if (in)
		file = "../cfg/in_config";

	return msgget(ftok(file, 1), 0666 | IPC_CREAT);
}

int send(int key, const void *buf)
{
	return msgsnd(key, buf, sizeof(buf), 0);
}

ssize_t receive(int key, void *msg, long pin)
{
	return msgrcv(key, msg, sizeof(msg), pin, 0);
}