#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include <devices/shutdown.h>
#include <threads/thread.h>
#include <filesys/filesys.h>

static void syscall_handler (struct intr_frame *);
void check_address(void* addr);
void get_argument(void *esp,int* arg,int count);
void halt();
void exit();
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{ 
  void* esp = f->esp;
  int sys_num = *(int*)(esp);
  int status,size;
  char* filename;
  int* arg[5];
  check_address(esp); 
  esp += 1;
  check_address(esp);
  switch(sys_num)
  {
    SYS_HALT: 
      halt();
      break; 	
    SYS_EXIT:
      get_argument(esp,arg,1)
      status=(int*)arg[0];
      exit(status);
      break;
    SYS_CREATE:
      get_argument(esp,arg,2);
      filename = (char*)*arg[0];
      size = (int)*arg[1];
      create(filename,size);
      break; 
    SYS_REMOVE:
      get_argument(esp,arg,1); 
      filename = (char*)*arg[0];
      remove(filename);
      break;
  } 
  printf ("system sycall!\n");
  thread_exit ();
}

void check_address(void* addr)
{
  if( addr <= 0x8048000 && addr >= 0xc0000000)
  {
    exit(-1);
  }

}
void halt()
{
  shutdown_power_off();
}

void exit(int status)
{ 
  struct thread *thread_cur=thread->current();
  printf("%s :exit(%d) \n",thread_cur->name,status);
  thread_exit();
}

bool create(const char *file, unsigned initial_size)
{
  return filesys_create(file,initial_size);
}
bool remove(const char* file){
  return filesys_remove(file);
}

void get_argument(void *esp, int * arg,int count)
{
  int i;
  for(i=0;i<count;i++)
  {
    arg[i] = esp+i+1;
    check_address(esp+i+1);
  } 
}














