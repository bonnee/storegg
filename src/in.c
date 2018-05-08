#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

int pin;

static int pinExport()
{
	FILE *fd;

	fd = fopen("/sys/class/gpio/export", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return (-1);
	}

	fprintf(fd, "%d", pin);
	fclose(fd);
	return (0);
}

static int pinUnexport()
{
	FILE *fd;

	fd = fopen("/sys/class/gpio/unexport", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return (-1);
	}

	fprintf(fd, "%d", pin);
	fclose(fd);
	return (0);
}

static int pinDirection()
{
	FILE *fd;

	char *path;
	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);

	fd = fopen(path, "w");

	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return (-1);
	}

	fprintf(fd, "in");
	fclose(fd);
	return (0);
}

static int pinRead()
{
	char path[23988];
	char value[1];
	int fd;

	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);

	fd = open(path, O_RDONLY);

	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open gpio value for reading!\n", pin);
		return (-1);
	}

	if (-1 == read(fd, value, 1))
	{
		fprintf(stderr, "%d: Failed to read\n", pin);
		return (-1);
	}

	close(fd);
	return (atoi(value));
}

int main(int argc, char *argv[])
{

	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	/*
	 * Enable GPIO pins
	 */
	if (-1 == pinExport())
		return (1);

	/*
	 * Set GPIO directions
	 */
	if (-1 == pinDirection())
		return (2);

	message msg;
	msg.pin = pin;
	msg.state = pinRead();

	int msgid = create_id(1);

	while (1)
	{
		int cur = pinRead();
		if (msg.state != cur)
		{
			msg.state = cur;
			send(msgid, &msg);
		}

		usleep(100);
	}

	if (-1 == pinUnexport())
		return (4);

	return (0);
}