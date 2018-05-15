#include "msg_queue.h"

int create_id(int id_queue)
{
	char *file;

	if (id_queue == 0)
	{
		file = "../tmp/out";
	}
	else if (id_queue == 1)
	{
		file = "../tmp/in";
	}
	else if (id_queue == 2)
	{
		file = "../tmp/keke"; //to change
	}
	else
	{
		printf("Wrong id_queue\n");
		return -1;
	}

	return msgget(ftok(file, 1), 0666 | IPC_CREAT);
}

int clear_queue(int key)
{
	return msgctl(key, IPC_RMID, NULL);
}

int send(int key, const void *buf, size_t size)
{
	return msgsnd(key, buf, size-sizeof(long), 0);
}

ssize_t receive(int key, void *msg, size_t size, long filter)
{
	return msgrcv(key, msg, size-sizeof(long), filter, 0);
}