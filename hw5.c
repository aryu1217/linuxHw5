#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char *args[20];
    char *word;
    pid_t pid;
    int status;

    while (1) {
        printf("$ ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        int i = 0;
        word = strtok(input, " ");
        while (word != NULL) {
            args[i++] = word;
            word = strtok(NULL, " ");
        }
        args[i] = NULL;

	if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "exit") == 0)
   		 break;

	if (strcmp(args[0], "cd") == 0) {
    		if (args[1] != NULL) {
        		if (chdir(args[1]) != 0) {
            			fprintf(stderr, "'chdir' to '%s' is failed.\n", args[1]);
       			}
    			} else {
        			printf("No input directory path.\n");
    			}
    		continue;
	}

        pid = fork();

        if (pid < 0) perror("fork failed");
        else if (pid == 0) {
            if (execvp(args[0], args) < 0) {
		fprintf(stderr, "execvp(%s) failed\n ", args[0]);
                exit(1);
            }
        } else {
            waitpid(pid, &status, 0);
            printf("(%d)\n", pid);
        }
    }
    return 0;
}

