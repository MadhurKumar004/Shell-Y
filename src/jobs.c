#include "jobs.h"
#include "header.h"


static job_t jobs[MAX_JOBS];
static int next_job_id = 1;

void init_jobs() {
    memset(jobs, 0, sizeof(jobs));
}

int add_job(pid_t pid, int is_background, char *cmdline) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == 0) {
            jobs[i].pid = pid;
            jobs[i].job_id = next_job_id++;
            jobs[i].is_background = is_background;
            strncpy(jobs[i].cmdline, cmdline, sizeof(jobs[i].cmdline) - 1);
            return jobs[i].job_id;
        }
    }
    return -1;
}

int remove_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == pid) {
            jobs[i].pid = 0;
            return 0;
        }
    }
    return -1;
}

void list_jobs() {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d] %s\n", jobs[i].job_id, jobs[i].cmdline);
        }
    }
}

void bring_job_to_foreground(int job_id) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].job_id == job_id) {
            tcsetpgrp(STDIN_FILENO, jobs[i].pid);
            kill(jobs[i].pid, SIGCONT);
            waitpid(jobs[i].pid, NULL, 0);
            tcsetpgrp(STDIN_FILENO, getpid());
            remove_job(jobs[i].pid);
            return;
        }
    }
    printf("No such job\n");
}

void send_job_to_background(int job_id) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].job_id == job_id) {
            kill(jobs[i].pid, SIGCONT);
            return;
        }
    }
    printf("No such job\n");
}

int get_next_job_id() {
    return next_job_id;
}