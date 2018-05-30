#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/thread.h"

struct lock filesys_lock;

void syscall_init (void);
void get_argument (void *, int *, int);
void check_address(void*);
void exit (int);
void halt (void);
bool create(const char*, unsigned);
bool remove(const char*);
tid_t exec(const char*);
int wait(tid_t tid);
int open(const char*);
int filesize(int fd);
int read(int, void*, unsigned);
int write(int, void*, unsigned);
void seek(int, unsigned);
unsigned tell(int);
void close(int);
#endif
