#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/thread.h"

void syscall_init (void);
void get_argument (void *, int *, int);
void check_address(void*);
void exit (int);
void halt (void);
bool create(const char*, unsigned);
bool remove(const char*);
tid_t exec(const char*);
int wait(tid_t tid);

#endif
