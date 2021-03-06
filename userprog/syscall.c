#include "userprog/syscall.h"
#include "userprog/process.h"

#include <stdio.h>
#include <syscall-nr.h>
#include "threads/synch.h"
#include "threads/interrupt.h"
<<<<<<< HEAD
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/file.h"
#include "filesys/filesys.h"

=======
#include "threads/thread.h"
>>>>>>> parent of 697a241... hierarchical structure done

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
<<<<<<< HEAD
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
    case SYS_OPEN:
      get_argument(esp, arg, 1);
      f->eax = open((const char*)arg[0]);
      break;
    case SYS_FILESIZE:
      get_argument(esp, arg, 1);
      f->eax = filesize((int)arg[0]);
      break;
    case SYS_READ:
      get_argument(esp, arg, 3);
      f->eax = read((int)arg[0], (void*)arg[1], (unsigned)arg[2]);
      break;
    case SYS_WRITE:
      get_argument(esp, arg, 3);
      f->eax = write((int)arg[0], (void*)arg[1], (unsigned)arg[2]);
      break;
    case SYS_SEEK:
      get_argument(esp, arg, 2);
      seek((int)arg[0], (unsigned)arg[1]);
      break;
    case SYS_TELL:
      get_argument(esp, arg, 1);
      f->eax = tell((int)arg[0]);
      break;
    case SYS_CLOSE:
      get_argument(esp, arg, 1);
      close((int)arg[0]);
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
=======
  printf ("system call!\n");
  thread_exit ();
>>>>>>> parent of 697a241... hierarchical structure done
}

int open (const char* file) {
  struct file *f;
  int fd;

  if(file == NULL)
    exit(-1);

  lock_acquire(&filesys_lock);

  f = filesys_open(file);
  if(!f) {
    lock_release(&filesys_lock);
    return -1;
  }

  fd = process_add_file(f);

  lock_release(&filesys_lock);

  return fd;
}

int filesize (int fd) {
  struct file* f = process_get_file(fd);
  if(f == NULL)
    return -1;
  return file_length(f);
}

int read (int fd, void* buffer, unsigned size) {
  lock_acquire(&filesys_lock);

  if(fd == 0) {
    int i = 0;
    for(;i<(int)size;i++)
      ((char*)buffer)[i] = input_getc();
  } else {  
    struct file* f = process_get_file(fd);
    if(f==NULL)
      size = -1;
    else
      size = file_read(f, buffer, size);
  }
  
  lock_release(&filesys_lock);

  return size;
}

int write (int fd, void* buffer, unsigned size) {
  lock_acquire(&filesys_lock);
  if(fd == 1) {
    putbuf(buffer,size);
  } else {
    struct file* f = process_get_file(fd);
    if(f == NULL)
      size = -1;
    else {
      size = file_write(f, buffer, size);
    }
  }
  lock_release(&filesys_lock);
  return size;
}

void seek (int fd, unsigned position) {
  struct file *f = process_get_file(fd);
  if(f != NULL) {
    file_seek(f, position);
  }
}

unsigned tell (int fd) {
  struct file *f = process_get_file(fd);

  if(f != NULL) {
    return file_tell(f);
  }
  return -1;

}

void close (int fd) {
  process_close_file(fd);
}

