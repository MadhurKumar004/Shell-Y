#include "exec.h"
#include "jobs.h"
#include "utils.h"
#include "parser.h"
#include "header.h"

void execute_cmd(char *cmds[][MAX_ARGS], int num_cmds) {
    int pipefd[2];
    pid_t pid;
    int fd_in = 0;
    int is_background = 0;

    for (int i = 0; i < num_cmds; i++) {
        // Check for the "cd" command and handle it separately
        if (strcmp(cmds[i][0], "cd") == 0) {
            if (cmds[i][1] == NULL) {
                // If no argument is passed, change to the home directory
                char *home_dir = getenv("HOME");
                if (home_dir == NULL) {
                    fprintf(stderr, "cd: HOME not set\n");
                } else {
                    if (chdir(home_dir) != 0) {
                        perror("cd failed");
                    }
                }
            } else {
                if (chdir(cmds[i][1]) != 0) {
                    perror("cd failed");
                }
            }
            continue; // Skip the rest of the loop for "cd" command
        }

        // Check if the command should run in the background
        int last_arg_index = 0;
        while (cmds[i][last_arg_index] != NULL) {
            last_arg_index++;
        }
        if (last_arg_index > 0 && strcmp(cmds[i][last_arg_index - 1], "&") == 0) {
            is_background = 1;
            cmds[i][last_arg_index - 1] = NULL;
        }

        pipe(pipefd);
        if ((pid = fork()) == -1) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            dup2(fd_in, 0);
            if (i < num_cmds - 1) {
                dup2(pipefd[1], 1);
            }
            close(pipefd[0]);
            handle_redirection(cmds[i]);
            execvp(cmds[i][0], cmds[i]);
            perror("execvp failed");
            exit(1);
        } else {
            if (is_background) {
                add_job(pid, 1, cmds[i][0]);
                printf("[%d] %d\n", get_next_job_id()  - 1, pid);
            } else {
                waitpid(pid, NULL, 0);
            }
            close(pipefd[1]);
            fd_in = pipefd[0];
        }
    }
}

void handle_redirection(char *args[]) {
    int in_fd = -1, out_fd = -1;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd == -1) {
                perror("open failed");
                exit(1);
            }
            dup2(out_fd, 1);
            close(out_fd);
            args[i] = NULL;
        } else if (strcmp(args[i], "<") == 0) {
            in_fd = open(args[i + 1], O_RDONLY);
            if (in_fd == -1) {
                perror("open failed");
                exit(1);
            }
            dup2(in_fd, 0);
            close(in_fd);
            args[i] = NULL;
        }
    }
}