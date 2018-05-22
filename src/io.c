#include "io.h"
 
//Ask the kernel to export the pin passed as parameter to userspace
int pinExport(int pin)
{
	FILE* fd;

	//open a special file in the filesystem
	fd = fopen("/sys/class/gpio/export", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open export for writing!\n", pin);
		return (-1);
	}

	//write into the file the number of the pin to export
	if (fprintf(fd, "%d", pin) <= 0)
	{
		fprintf(stderr, "%d: Failed to print on export!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}

//Ask the kernel to reverse the export function
int pinUnexport(int pin)
{
	FILE* fd;

	//open a special file in the filesystem
	fd = fopen("/sys/class/gpio/unexport", "w");
	if (NULL == fd)
	{
		fprintf(stderr, "%d: Failed to open unexport for writing!\n", pin);
		return (-1);
	}

	//write into the file the number of the pin to unexport
	if (fprintf(fd, "%d", pin) <= 0)
	{
		fprintf(stderr, "%d: Failed to print on unexport!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}

//sets the direction of the specified pin, input or output pin
int pinDirection(int pin, int mode)
{
	//create a string that contains the direction passed as param
	char *dir = "in";
	if (mode)
	{
		dir = "out";
	}

	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	FILE* fd;

	//try to open a special file referring the specified gpio pin created by the export function
	sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
	do {
		fd = fopen(path, "w");
		usleep(100000); //waits to avoid concurrency of processes
	} while (fd == NULL);

	//write the direction into the direction file
	if (fprintf(fd, "%s", dir) <= 0)
	{
		fprintf(stderr, "%d: Failed to open direction for writing!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}

//read the value of a specified pin, value: 0, 1
int pinRead(int pin)
{	
	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	FILE* fd;
	//open a special file referring the specified gpio pin created by the export function
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
	fd = fopen(path, "r");

	if (fd == NULL){
		fprintf(stderr, "%d: Failed to open gpio value for reading!\n", pin);
		return (-1);
	}
	char* l;
    size_t size = 10;
    l = (char *)malloc(size * sizeof(char));

	//read the value of the gpio pin
	if (getline(&l,&size,fd) <= 0)
	{
		fprintf(stderr, "%d: Failed to read direction!\n", pin);
		return -1;
	}
	fclose(fd);
	return (atoi(l));
}

//write the value of a specified pin, value: 0, 1
int pinWrite(int pin, int value)
{
	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	FILE* fd;
	//open a special file referring the specified gpio pin created by the export function
	sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
	fd = fopen(path, "w");

	if (fd == NULL){
		fprintf(stderr, "%d: Failed to open gpio value for writing!\n", pin);
		return (-1);
	}

	//read the value for the gpio pin
	if (fprintf(fd, "%d",value) <= 0)
	{
		fprintf(stderr, "%d: Failed to print pin value!\n", pin);
		return -1;
	}
	fclose(fd);
	return (0);
}