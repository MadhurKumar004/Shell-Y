#include "parser.h"
#include "header.h"

int parse_input(char *input, char *cmds[][MAX_ARGS]) {
    char *token;
    int arg_count = 0;
    int cmd_index = 0;

    input[strcspn(input, "\n")] = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        if (strcmp(token, "|") == 0) {
            cmds[cmd_index][arg_count] = NULL;
            cmd_index++;
            arg_count = 0;
        } else {
            cmds[cmd_index][arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    cmds[cmd_index][arg_count] = NULL;
    return cmd_index + 1;
}