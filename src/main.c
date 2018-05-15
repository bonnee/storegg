#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    if(argc!=2){
        printf("Invalid number of parameters");
        return 1;
    }

    int n_eggs = atoi(argv[1]);
    int lines = 0;

    char* path = "../cfg/in_config";

    FILE* f = fopen(path, "r");
    char* l;
    size_t size = 10;
    l = (char *)malloc(size * sizeof(char));
    while(!feof(f)){
        getline(&l,&size,f);
        lines++;
    }

    lines = lines - 2;

    if(n_eggs != lines) {
        printf("Not enough pins");
        return 2;
    }

    if(fork()==0){
        char str[2];
		sprintf(str, "%d", n_eggs);

		char *args[] = {"./handler", str, NULL};
        printf("Sono il figlio");
		execvp(args[0], args);
    }

    wait(NULL);

    return 0;
}
