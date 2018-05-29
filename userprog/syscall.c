#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "devices/shutdown.h"
#include "filesys/filesys.h"
#include "threads/synch.h"
#include "userprog/process.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

// check addr within user-addr
void check_address(void *addr) {
  if(!((void*)0x80480000 <= addr && addr <= (void*)0xc0000000))
    exit(-1);
  return; 
}

// save argument to kernel from userstacks
void get_argument(void *esp, int *arg, int count) {
  int i;
  int *tmp = esp;

  check_address(esp);
  check_address(esp + (count * 4));

  for(i=0;i<count;i++)
    arg[i] = *(int*)(++tmp);
}	

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  uint32_t *esp = f->esp;
  check_address((void*)esp);

  int syscall_n = *esp;
  int arg[5];

  switch(syscall_n) {
    case SYS_HALT:
      halt();
      break;
    case SYS_EXIT:
      get_argument(esp, arg, 1);
      exit((int)arg[0]);
      break;
    case SYS_CREATE:
      get_argument(esp, arg, 2);
      f->eax = create((const char*)arg[0], (unsigned)arg[1]);
      break;
    case SYS_REMOVE:
      get_argument(esp, arg, 1);
      f->eax = remove((const char*)arg[0]);
      break;
    case SYS_EXEC:
      get_argument(esp, arg, 1);
      f->eax = remove((const char*)arg[0]);
      break;
    case SYS_WAIT:
      get_argument(esp, arg, 1);
      f->eax = wait((tid_t)arg[0]);
      break;
  }
}

void halt(void) {
  thread_exit();
  shutdown_power_off();
}

void exit(int status) {
  struct thread *t = thread_current();
  printf("exit status : %d\n", status);
  t->exit_status = status;
  thread_exit();
}

bool create(const char* file, unsigned initial_size) {
  if(file == NULL)
    exit(-1);
  return filesys_create(file, initial_size);
}

bool remove(const char* file) {
  return filesys_remove(file);
}

tid_t exec(const char* cmd_line) {
  tid_t tid = process_execute(cmd_line);
  struct thread* child = get_child_process(tid);
  sema_down(&child->load_sema);
  if(child->loaded)
    return tid;
  return -1;
}

int wait(tid_t tid) {
  return process_wait(tid);
}
