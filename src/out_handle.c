#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PATH "../cfg/out_config"
#define N 8

int pins[N];
int values[N];

int main(int argc, char *argv[])
{
	FILE *pinfile;

	char *line = NULL;
	size_t len = 0;

	pinfile = fopen(PATH, "r");

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
			char str[2];
			sprintf(str, "%d", pins[i]);

			char *args[] = {"./out", str, NULL};
			execvp(args[0], args);
		}
	}

	wait(NULL);
	return 0;
}