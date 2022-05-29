#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wait.h>
#include <unistd.h>

#include "header.h"

extern char** environ;
extern const char child_path[];

int main(int argc, char* argv[], char* envp[]) {
    if (argc != 2) {
        fprintf(stderr, "%d is invalid amount of arguments, must be 2\n", argc);
        exit(EXIT_FAILURE);
    }

    print_envp(envp);
    char* const* env = create_child_env(argv[1]); // argv[1] is env file path

    size_t child_count = 0;
    int    opt;
    while ((opt = getchar()) != EOF) {
        if (opt == 'q') {
            exit(EXIT_SUCCESS);
        }
        if (opt != '+' && opt != '*' && opt != '&') {
            continue;
        }

        // Get path to child process
        char* child_process = NULL;
        switch ((char) opt) {
            case '+':
                child_process = getenv(child_path);
                break;

            case '*':
                child_process = search_child_path(envp);
                break;

            case '&':
                child_process = search_child_path(environ);
                break;
        }

        // Create new child name
        char child_name[10];
        sprintf(child_name, "child_%zu", child_count++);

        char* const args[] = {child_name, argv[1], NULL};

        pid_t pid = fork();
        if (pid > 0) {
            int status;
            wait(&status);
        } else if (pid == 0) {
            if (execve(child_process, args, env) == -1) {
                perror("execve");
                exit(errno);
            }
        } else {
            perror("fork");
            exit(errno);
        }
    }
}
