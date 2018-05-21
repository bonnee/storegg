#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// this directory will only exists if the program is run on a Raspberry 
#define RASPI_PATH "/sys/firmware/devicetree/base/model"
#define IN_CONFIG "../src/cfg/in_config"
#define OUT_CONFIG "../src/cfg/out_config"

int detect_raspi()
{
    int raspi = 0;
    char *check_string = "Raspberry Pi";

    //if this file opens it means we're working on a Raspberry, so we can use the gpio
    FILE *f = fopen(RASPI_PATH, "r");

    if (f == NULL) {
        printf("Error in opening the Raspberry file \n");
        return 8;
    }

    if (f)
    {
        size_t size = sizeof(check_string);
        char *l = (char *)malloc(size * sizeof(char));

        while (!feof(f))
        {
            getline(&l, &size, f);

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

int count_lines()
{
    //counting the lines of the in_config file to detect if there are enough pins
    int lines = 0;

    FILE *f = fopen(IN_CONFIG, "r");
    if (f == NULL) {
        printf("Error in opening the config file \n");
        return 7;
    }
    
    if (f)
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
// subtracting the 2 storage's pins
    return lines - 2;
}

int main(int argc, char const *argv[])
{
    printf("storegg loading...\n");

    // some checks before starting the real program
    if (argc != 2)
    {
        printf("Invalid number of parameters. Usage: ./main n_eggs\n");
        return 1;
    }

    if (!detect_raspi())
    {
        printf("This program is meant to run on a Raspberry Pi.\n");
        return 5;
    }

    int n_eggs = atoi(argv[1]);
    int lines = count_lines();

    if (n_eggs != lines)
    {
        printf("Wrong number of pins\n");
        return 2;
    }

    // forking the child that will run the process handler and checking if the fork works
    pid_t f_val = fork();
    if (f_val < 0)
    {
        printf("Can't fork handler\n");
        return 3;
    }

    if (f_val == 0)
    {
        // passing as parameters of the handler the the config file and the number of eggs
        char egg_num[2];
        sprintf(egg_num, "%d", n_eggs);

        char *args[] = {"./handler", OUT_CONFIG, IN_CONFIG, egg_num, NULL};
        if (execvp(args[0], args) == -1)
        {
            return 4;
        };
    }

    //the parent process waits till the child process is done
    wait(NULL);
    return 0;
}
