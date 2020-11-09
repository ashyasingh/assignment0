//objsect
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <bfd.h>

static char hex_to_ascii(unsigned char hex)
{
  if (hex >= 0 && hex <= 9)
    return (48+hex);    //ascii 0-9
  else if (hex >= 0xa && hex <= 0xf)
    return (97+hex-0xa);    //ascii a-f
  else
    return (32);  //print space - this is invalid char
}

static void print_space(int out_fd)
{
  char space_char = 32;
  write(out_fd, &space_char, sizeof(space_char));
}

static void print_newline(int out_fd)
{
  char newline_chars[2] = {10,13};
  write(out_fd, newline_chars, sizeof(newline_chars));
}

//this function doesn't truncate
static void print_str(int out_fd, const char *str, int align_len)
{
  int i;
  int str_len = strlen(str);

  write(out_fd, str, str_len);
  for (i = str_len; i < align_len; i++)
  {
    print_space(out_fd);
  }
}

static void print_hex64(int out_fd, unsigned long hex_val)
{
  unsigned char hex_byte;
  char hex_chars[2];
  int i;

  for (i = 0; i < 8; i++)
  {
    hex_byte = (hex_val >> ((7-i)*8)) & 0xff;
    hex_chars[0] = hex_to_ascii((hex_byte >> 4) & 0xf);
    hex_chars[1] = hex_to_ascii(hex_byte & 0xf);
    write(out_fd, hex_chars, sizeof(hex_chars));
  }
}

static void objsect_print(bfd *abfd, asection *asec, void *arg)
{
  int out_fd = *(int*)arg;

  print_str(out_fd, bfd_section_name(abfd, asec), 32);
  print_hex64(out_fd, bfd_section_vma(abfd, asec));
  print_space(out_fd);
  print_hex64(out_fd, bfd_section_size(abfd, asec));
  print_space(out_fd);
  print_hex64(out_fd, asec->filepos);
  print_newline(out_fd);
}

int get_objsect (const char* infile)
{
  int out_fd = 1; //stdout
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

  print_str(out_fd, "name", 32);
  print_str(out_fd, "vma", 16+1);
  print_str(out_fd, "size", 16+1);
  print_str(out_fd, "position", 16);
  print_newline(out_fd);

  bfd_map_over_sections(abfd, objsect_print, &out_fd);

  bfd_close(abfd);

  return 0;
}

