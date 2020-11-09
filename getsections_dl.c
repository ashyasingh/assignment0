#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dlfcn.h>

#define RDTSC(var)  \
  { \
    unsigned long var##_lo, var##_hi;    \
    asm volatile("lfence\n\trdtsc" : "=a"(var##_lo), "=d"(var##_hi));   \
    var = var##_hi; \
    var <<= 32;  \
    var |= var##_lo;    \
  }

extern int get_objsect (const char* infile);
int (*get_objsect_ptr) (const char* infile);

int main (int argc, char *argv[])
{
  int dl_mode = 0;
  unsigned long long start, finish;

  if(argc != 3)
  {
    printf("usage: %s <filename> (RTLD_LAZY | RTLD_NOW)\n", argv[0]);
    return -1;
  }

  if(strcmp(argv[2], "RTLD_LAZY") == 0)
    dl_mode = RTLD_LAZY;
  else if (strcmp(argv[2], "RTLD_NOW") == 0)
    dl_mode = RTLD_NOW;
  else
  {
    printf("usage: %s <filename> (RTLD_LAZY | RTLD_NOW)\n", argv[0]);
    return -1;
  }
  
  void* lib_handle;

  RDTSC(start);
  lib_handle = dlopen("libobjdata.so", dl_mode | RTLD_GLOBAL);
  RDTSC(finish);
  if(!lib_handle)
  {
    printf("dlopen error - %s\n", dlerror());
    return -1;
  }

  const char* err_msg;
  get_objsect_ptr = dlsym(lib_handle, "get_objsect");
  err_msg = dlerror();
  if(err_msg)
  {
    printf("dlsym error - %s\n", err_msg);
    return -1;
  }

  (*get_objsect_ptr) (argv[1]);

  dlclose(lib_handle);

  //cpu MHz     : 2194.918
  unsigned long long cpu_mhz = 2195;
  unsigned long long dlopen_cycles = (finish - start);
  printf("dlopen_usecs %llu\n", dlopen_cycles/cpu_mhz);

  return 0;
}
