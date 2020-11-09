#include <stdio.h>
#include <errno.h>

extern int sect_objcopy (const char* infile, const char* match_str, const char* outfile);

int main (int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("usage: %s <infile> \n", argv[0]);
    return -1;
  }
  
  const char *match_str = ".text";
  const char *outfile = "text-output";
  sect_objcopy (argv[1], match_str, outfile);
    
  return 0;
}
