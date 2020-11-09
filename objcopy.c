//objcopy
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <bfd.h>
#include <sys/stat.h>
#include <fcntl.h>

static bfd_boolean sect_objcopy_write(bfd *abfd, asection *asec, void *arg)
{
  int out_fd = *(int*)arg;

  bfd_byte *sect_data;
  unsigned long sect_size = bfd_section_size(abfd, asec);

  if (bfd_malloc_and_get_section(abfd, asec, &sect_data) == TRUE)
  {
    write(out_fd, sect_data, sect_size);
    free(sect_data);
  }
  return FALSE; //keep looking for next match
}

int sect_objcopy (const char* infile, const char* match_str, const char* outfile)
{
  int out_fd = -1;
  bfd *abfd;

  bfd_init();

  abfd = bfd_openr (infile, "elf64-x86-64");
  if (abfd == NULL)  
  {
    bfd_perror("bfd_openr error");
    return -1;
  }

  if (bfd_check_format(abfd, bfd_object) != TRUE)
  {
    bfd_close(abfd); 
    return -1;
  }

  out_fd = open(outfile, O_CREAT | O_WRONLY);
  if (out_fd == -1)
  {
    bfd_close(abfd); 
    return -1;
  }

  bfd_get_section_by_name_if(abfd, match_str, sect_objcopy_write, &out_fd);

  close(out_fd);
  bfd_close(abfd);

  return 0;
}

