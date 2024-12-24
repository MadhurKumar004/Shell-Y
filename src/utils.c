#include "utils.h"
#include "header.h"

void sigchld_handler(int sig) {
    
    wait(NULL);
}

