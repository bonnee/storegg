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
		fprintf(stderr, "Failed to open export for writing!\n");
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
		fprintf(stderr, "Failed to open unexport for writing!\n");
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
		fprintf(stderr, "Failed to open %d direction for writing!\n", pin);
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
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return (-1);
	}

	if (-1 == read(fd, value, 1))
	{
		fprintf(stderr, "Failed to pinRvalue!\n");
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

	int status = pinRead();

	do
	{
		int cur = pinRead();
		if (status != cur)
		{
			status = cur;
			printf("IL VALORE È CAMBIATO!!!11!\n");
		}

		usleep(100);
	} while (1);

	if (-1 == pinUnexport())
		return (4);

	return (0);
}