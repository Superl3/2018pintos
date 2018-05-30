#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

void argument_stack(char**, int, void**);
tid_t process_execute (const char *file_name);
struct thread *get_child_process (int pid);
void remove_child_process(struct thread*);
int process_add_file (struct file*);
struct file* process_get_file (int);
void process_close_file (int);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
