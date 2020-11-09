#include <stdio.h>
#include <errno.h>

extern int get_objsect (const char* infile);

int main (int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("usage: %s <filename> \n", argv[0]);
    return -1;
  }
  
  get_objsect (argv[1]);
    
  return 0;
}
