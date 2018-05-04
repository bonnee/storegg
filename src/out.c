#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LOW 0
#define HIGH 1

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

	fprintf(fd, "out");
	fclose(fd);
	return (0);
}

static int pinWrite(int value)
{
	FILE *fd;

	char path[23988];
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);

	fd = fopen(path, "w");

	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open file for writing!\n", pin);
		return (-1);
	}

	fprintf(fd, "%d", value);
	fclose(fd);
	return (0);
}

int main(int argc, char *argv[])
{
	if (argc <= 1)
		return -1;

	pin = atoi(argv[1]);
	printf("Pin: %d\n", pin);

	if (-1 == pinExport())
		return (1);

	if (-1 == pinDirection())
		return (2);

	int status = LOW;

	do
	{
		printf("writing %d\n", status);
		pinWrite(status);

		status = !status;

		sleep(1);
	} while (1);

	if (-1 == pinUnexport())
		return (4);

	return (0);
}