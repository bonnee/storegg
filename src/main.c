#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RASPI_PATH "/sys/firmware/devicetree/base/model"
#define IN_CONFIG "../src/cfg/in_config"
#define OUT_CONFIG "../src/cfg/out_config"

char *in_config, *out_config;

int detect_raspi()
{
    int raspi = 0;
    char *check_string = "Raspberry Pi";

    FILE *f = fopen(RASPI_PATH, "r");

    if (f)
    {
        size_t size = sizeof(check_string);
        char *l = (char *)malloc(size * sizeof(char));

        while (!feof(f))
        {
            getline(&l, &size, f);

            // If file contains "Raspberry Pi" we're happy
            if (strstr(l, check_string))
            {
                raspi = 1;
            }
        }
        free(l);
        fclose(f);
    }

    return raspi;
}

// Counts in_config lines
int count_lines()
{
    int lines = 0;

    FILE *f = fopen(in_config, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error in opening the config file \n");
        return -1;
    }
    else
    {
        size_t size = 10;
        char *l = (char *)malloc(size * sizeof(char));

        while (!feof(f))
        {
            getline(&l, &size, f);
            lines++;
        }
        free(l);
        fclose(f);
    }
    // We only need # of eggs
    return lines - 2;
}

int main(int argc, char const *argv[])
{
    // Check parameters
    if (argc != 2 && argc != 4)
    {
        fprintf(stderr, "Invalid number of parameters.\nUsage: %s N_EGGS [IN_CONFIG] [OUT_CONFIG]\n", argv[0]);
        return 1;
    }

    // if the user passes 4 arguments, it uses the in/out config passed
    if (argc == 4)
    {
        in_config = (char *)malloc(sizeof(argv[2]));
        out_config = (char *)malloc(sizeof(argv[3]));
        strcpy(in_config, argv[2]);
        strcpy(out_config, argv[3]);
    }
    else // uses the default in/out config
    {
        in_config = IN_CONFIG;
        out_config = OUT_CONFIG;
    }

    if (!detect_raspi())
    {
        fprintf(stderr, "This program is meant to run on a Raspberry Pi.\n");
        return 5;
    }

    int n_eggs = atoi(argv[1]);
    int lines = count_lines();

    if (n_eggs != lines)
    {
        fprintf(stderr, "Wrong number of pins\n");
        return 2;
    }

    // forking the child that will run the process handler and checking if the fork works
    pid_t f_val = fork();
    if (f_val < 0)
    {
        fprintf(stderr, "Can't fork handler\n");
        return 3;
    }

    if (f_val == 0)
    {
        // passes n_eggs and config file to handler
        char egg_num[2];
        sprintf(egg_num, "%d", n_eggs);

        char *args[] = {"./handler", out_config, in_config, egg_num, NULL};
        if (execvp(args[0], args) == -1)
        {
            fprintf(stderr, "Exec didn't work!\n");
            return 4;
        };
    }

    //the parent process waits till the child process is done
    wait(NULL);
    return 0;
}
