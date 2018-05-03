#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PATH "in_config"
#define N 8

// 0-5 eggs
// 6-7 storage
int pins[N];
int values[N];

int main(int argc, char *argv[])
{
	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	pinfile = fopen("in_config", "r");

	if (NULL == pinfile)
	{
		printf("No config\n");
		return -1;
	}

	for (int i = 0; i < N; i++)
	{
		getline(&line, (size_t *)&len, pinfile);
		pins[i] = atoi(line);
	}
	fclose(pinfile);

	for (int i = 0; i < N; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			printf("kekko\n");
			char str[2];
			sprintf(str, "%d", pins[i]);

			char *args[] = {"./in", str, NULL};
			execvp(args[0], args);
		}
	}

	wait(NULL);
	return 0;
}