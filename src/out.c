#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"

#define LOW 0
#define HIGH 1

int pin;

static int pinExport()
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open export for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);

	/*int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return (-1);
	}

	char *value;
	sprintf(value, "%d", pin);
	if (-1 == write(fd, value, strlen(value)))
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return -1;
	}
	close(fd);
	return (0);*/
}

static int pinUnexport()
{
	printf("enter pinUnexport\n");
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return (-1);
	}

	char *value;
	sprintf(value, "%d", pin);
	if (-1 == write(fd, value, strlen(value)))
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return -1;
	}

	close(fd);
	printf("Leave pinUnexport\n");
	return (0);
}

static int pinDirection()
{
	printf("Enter pinDirection\n");
	static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open gpio direction for writing!\n", pin);
		return (-1);
	}

	if (-1 == write(fd, &s_directions_str[3], 3))
	{
		fprintf(stderr, "%d: Failed to set direction!\n", pin);
		return (-1);
	}

	close(fd);
	printf("Leave pinDirection\n");
	return (0);
	/*	int fd;

	char *path;
	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);

	fd = open(path, O_WRONLY);

	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return (-1);
	}
	if (-1 == write(fd, "out", 3))
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return -1;
	}

	close(fd);
	return (0);*/
}

static int pinWrite(int value)
{
	static const char s_values_str[] = "01";

	char path[40];
	int fd;

	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return (-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1))
	{
		fprintf(stderr, "Failed to write value!\n");
		return (-1);
	}

	close(fd);
	return (0);
	/*int fd;

	char path[23988];
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);

	fd = open(path, O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "%d: Failed to open file for writing!\n", pin);
		return -1;
	}

	char *str;
	sprintf(str, "%d", value);
	if (-1 == write(fd, &str, strlen(str)))
	{
		fprintf(stderr, "%d: Failed to open pin for writing!\n", pin);
		return -1;
	}
	close(fd);
	return (0);*/
}

int main(int argc, char *argv[])
{
	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	if (-1 == pinExport())
		return (1);

	sleep(1); // brutto

	if (-1 == pinDirection())
		return (2);

	int status = LOW;

	message msg;
	int msgid = create_id(0);

	while (1)
	{
		receive(msgid, &msg, pin);
		printf("%d received %d: %d\n", pin, msg.pin, msg.state);
		pinWrite(msg.state);
	}

	if (-1 == pinUnexport())
		return (4);

	return (0);
}