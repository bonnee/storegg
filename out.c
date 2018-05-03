#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define IN 0
#define OUT 1

#define LOW 0
#define HIGH 1

#define PIN 18

static int GPIOExport(int pin)
{
	FILE *fd;

	fd = fopen("/sys/class/gpio/export", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "Failed to open export for writing!\n");
		return (-1);
	}

	fprintf(fd, "%d", pin);
	fclose(fd);
	return (0);
}

static int GPIOUnexport(int pin)
{
	FILE *fd;

	fd = fopen("/sys/class/gpio/unexport", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return (-1);
	}

	fprintf(fd, "%d", pin);
	fclose(fd);
	return (0);
}

static int GPIODirection(int pin)
{
	FILE *fd;

	char *path;
	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);

	fd = fopen(path, "w");

	if (NULL == fd)
	{
		fprintf(stderr, "Failed to open %d direction for writing!\n", pin);
		return (-1);
	}

	fprintf(fd, "out");
	fclose(fd);
	return (0);
}

static int GPIOWrite(int pin, int value)
{
	FILE *fd;

	char path[23988];
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);

	fd = fopen(path, "w");

	if (NULL == fd)
	{
		fprintf(stderr, "Failed to open file for writing!\n");
		return (-1);
	}

	fprintf(fd, "%d", value);
	fclose(fd);
	return (0);
}

int main(int argc, char *argv[])
{
	/*
	 * Enable GPIO pins
	 */
	if (-1 == GPIOExport(PIN))
		return (1);

	/*
	 * Set GPIO directions
	 */
	if (-1 == GPIODirection(PIN))
		return (2);

	int status = 0;

	do
	{
		printf("writing %d\n", status);
		GPIOWrite(PIN, status);

		status = !status;

		sleep(1);
	} while (1);

	if (-1 == GPIOUnexport(PIN))
		return (4);

	return (0);
}