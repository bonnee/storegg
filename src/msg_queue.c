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
		file = "../tmp/keke"; //to change
	}
	else
	{
		printf("Wrong id_queue\n");
		return -1;
	}

	return msgget(ftok(file, 1), 0666 | IPC_CREAT);
}

//it deletes the msg queue specified as argument
int clear_queue(int key)
{
	int res = msgctl(key, IPC_RMID, NULL);
	if (res < 0){
		printf("Failed to delete queue");
		return -1;
	} else {
		return res;
	}
	//return msgctl(key, IPC_RMID, NULL);
}

//function that wraps the send for the message queue
int send(int key, const void *buf, size_t size)
{	
	int res = msgsnd(key, buf, size-sizeof(long), 0);
	if (res < 0){
		printf("Failed to send message");
		return -1;
	} else {
		return res;
	}
	//return msgsnd(key, buf, size-sizeof(long), 0);
}

//function that wraps the receive for the message queues
ssize_t receive(int key, void *msg, size_t size, long filter)
{
	int res = msgrcv(key, msg, size-sizeof(long), filter, 0);
	if (res < 0){
		printf("Failed to receive message");
		return -1;
	} else {
		return res;
	}
	//return msgrcv(key, msg, size-sizeof(long), filter, 0);
}