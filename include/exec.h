#ifndef EXEC_H
#define EXEC_H

#define MAX_ARGS 50 

void execute_cmd(char *cmds[][MAX_ARGS], int num_cmds);
void handle_redirection(char *args[]);

#endif // EXEC_H