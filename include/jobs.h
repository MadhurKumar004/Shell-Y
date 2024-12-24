#ifndef JOBS_H
#define JOBS_H

#include "header.h"
#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    int job_id;
    char cmdline[1024];
    int is_background;
} job_t;

void init_jobs();
int add_job(pid_t pid, int is_background, char *cmdline);
int remove_job(pid_t pid);
void list_jobs();
void bring_job_to_foreground(int job_id);
void send_job_to_background(int job_id);
int get_next_job_id();

#endif // JOBS_H