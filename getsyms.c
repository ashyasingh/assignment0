#include <stdio.h>
#include <errno.h>

extern int get_objsym (const char* infile);

int main (int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("usage: %s <filename> \n", argv[0]);
    return -1;
  }
  
  get_objsym (argv[1]);
    
  return 0;
}
