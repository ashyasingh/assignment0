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

extern int sect_objcopy (const char* infile, const char* match_str, const char* outfile);
int (*sect_objcopy_ptr) (const char* infile, const char* match_str, const char* outfile);

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
  sect_objcopy_ptr = dlsym(lib_handle, "sect_objcopy");
  err_msg = dlerror();
  if(err_msg)
  {
    printf("dlsym error - %s\n", err_msg);
    return -1;
  }

  const char *match_str = ".text";
  const char *outfile = "text-output-dl";
  (*sect_objcopy_ptr) (argv[1], match_str, outfile);

  dlclose(lib_handle);

  //cpu MHz     : 2194.918
  unsigned long long cpu_mhz = 2195;
  unsigned long long dlopen_cycles = (finish - start);
  printf("dlopen_usecs %llu\n", dlopen_cycles/cpu_mhz);

  return 0;
}
