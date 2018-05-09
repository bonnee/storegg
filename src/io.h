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

static int pinExport(int pin)
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

static int pinUnexport(int pin)
{
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
	return (0);
}

static int pinDirection(int pin, int mode)
{

	char *dir = "in";
	if (mode)
	{
		dir = "out";
	}

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

	if (-1 == write(fd, dir, strlen(dir)))
	{
		fprintf(stderr, "%d: Failed to set direction!\n", pin);
		return (-1);
	}

	close(fd);
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

static int pinRead(int pin)
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

static int pinWrite(int pin, int value)
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