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
	FILE* fd;

	fd = fopen("/sys/class/gpio/export", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return (-1);
	}

	if (fprintf(fd, "%d", pin) <= 0)
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}

static int pinUnexport(int pin)
{
	FILE* fd;

	fd = fopen("/sys/class/gpio/unexport", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return (-1);
	}

	if (fprintf(fd, "%d", pin) <= 0)
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return -1;
	}
	fclose(fd);
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
	FILE* fd;
	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
	do {
		fd = fopen(path, "w");
		usleep(100000);
	} while (fd == NULL);

	if (fprintf(fd, dir) <= 0)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}

static int pinRead(int pin)
{
	
	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	FILE* fd;
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
	fd = fopen(path, "r");

	if (fd == NULL){
		fprintf(stderr, "%d: Failed to open gpio value for reading!\n", pin);
		return (-1);
	}
	char* l;
    size_t size = 10;
    l = (char *)malloc(size * sizeof(char));

	if (getline(&l,&size,fd) <= 0)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return -1;
	}
	fclose(fd);
	return (atoi(l));
}

static int pinWrite(int pin, int value)
{
	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	FILE* fd;
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
	fd = fopen(path, "w");

	if (fd == NULL){
		fprintf(stderr, "%d: Failed to open gpio value for writing!\n", pin);
		return (-1);
	}

	if (fprintf(fd, "%d",value) <= 0)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}