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

#define PIN 4

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

	fprintf(fd, "in");
	fclose(fd);
	return (0);
}

static int GPIORead(int pin)
{
	char path[23988];
	char value[1];
	int fd;

	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);

	fd = open(path, O_RDONLY);

	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return (-1);
	}

	if (-1 == read(fd, value, 1))
	{
		fprintf(stderr, "Failed to read value!\n");
		return (-1);
	}

	close(fd);
	return (atoi(value));
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

	int status = GPIORead(PIN);

	do
	{
		int cur = GPIORead(PIN);
		if (status != cur)
		{
			status = cur;
			printf("IL VALORE È CAMBIATO!!!11!\n");
		}

		usleep(100);
	} while (1);

	if (-1 == GPIOUnexport(PIN))
		return (4);

	return (0);
}