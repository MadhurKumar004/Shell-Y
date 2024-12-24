#include "exec.h"
#include "header.h"
#include "utils.h"
#include "parser.h"
//#include "signals.h"
#include "jobs.h"
//#include "history.h"
#include "env.h"

int main() {
    char input[1024];  // Buffer to hold the input
    char *cmds[10][MAX_ARGS]; // Array of commands for pipeline
    int should_run = 1; // Flag to control the shell loop

    signal(SIGINT, sigchld_handler); // Handle Ctrl+C
    signal(SIGTSTP, sigchld_handler); // Handle Ctrl+Z

    init_jobs(); // Initialize job list

    while (should_run) {
        printf("shell> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            break; 
        }

        int num_cmds = parse_input(input, cmds);

        if (num_cmds == 0) {
            printf("No command entered. Try again.\n");
            continue;
        }

        // Check for the "exit" command to terminate the shell
        if (strcmp(cmds[0][0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Check for job control commands
        if (strcmp(cmds[0][0], "jobs") == 0) {
            list_jobs();
            continue;
        } else if (strcmp(cmds[0][0], "fg") == 0) {
            if (cmds[0][1] != NULL) {
                bring_job_to_foreground(atoi(cmds[0][1]));
            } else {
                printf("fg: job id required\n");
            }
            continue;
        } else if (strcmp(cmds[0][0], "bg") == 0) {
            if (cmds[0][1] != NULL) {
                send_job_to_background(atoi(cmds[0][1]));
            } else {
                printf("bg: job id required\n");
            }
            continue;
        }

        execute_cmd(cmds, num_cmds);
    }

    return 0;
}