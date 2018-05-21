#ifndef MSG_QUEUE
#define MSG_QUEUE

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>

#define MAX_NUM_EGGS 20
#define OUT_FILE "../src/queue/out"
#define IN_FILE "../src/queue/in"
#define HANDLER_FILE "../src/queue/handler"

typedef struct hw_buffer
{
	long pin;  // physical pin
	int state; //value, bit 1/0
} message;

typedef struct sw_buffer
{
	long type;
	int state[MAX_NUM_EGGS]; //values of all the input or output pins 1/0
} swbuffer;

int create_id(int id_queue);

int clear_queue(int key);

int send(int key, const void *buf, size_t size);

ssize_t receive(int key, void *msg, size_t size, long filter);

#endif