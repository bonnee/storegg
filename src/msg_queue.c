#include "msg_queue.h"

int create_id(int id_queue)
{
	char *file;
	//depending on the number passed as parameter it uses a different file to create the key of the queue
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
		file = "../tmp/handler";
	}
	else
	{
		printf("Wrong id_queue\n");
		return -1;
	}
	/*	// da testare con raspberry
	key_t f_val = ftok(file, 1);
	if (f_val == -1) {
		// gestibile con errno, volendo, e anche il msgget
	}
	int m_val = msgget(f_val, 0666 | IPC_CREAT);
	if (m_val == -1) {
		// gestione
	}
	return m_val;
	*/
	return msgget(ftok(file, 1), 0666 | IPC_CREAT);
}

//it deletes the msg queue specified as argument
int clear_queue(int key)
{
	int res = msgctl(key, IPC_RMID, NULL);
	if (res < 0)
	{
		printf("Failed to delete queue");
		return -1;
	}
	else
	{
		return res;
	}
	//return msgctl(key, IPC_RMID, NULL);
}

//function that wraps the send for the message queue
int send(int key, const void *buf, size_t size)
{
	int res = msgsnd(key, buf, size - sizeof(long), 0);
	if (res < 0)
	{
		printf("Failed to send message");
		return -1;
	}

	return res;
}

//function that wraps the receive for the message queues
ssize_t receive(int key, void *msg, size_t size, long filter)
{
	int res = msgrcv(key, msg, size - sizeof(long), filter, 0);
	if (res < 0)
	{
		printf("Failed to receive message");
		return -1;
	}

	return res;
}