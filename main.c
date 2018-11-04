#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  if(argc < 2)
    {
      printf("Usage: main size\n");
      exit(1);
    }
  int size = atoi(argv[1])*1024;
  int rc   = mem_init(size, 0);
  if(rc != 0)
    exit(1);

  char *p   = (char*) mem_alloc(sizeof(char));
  mem_dump();
  char* x  = (char*) mem_alloc(sizeof(char));
  mem_dump();
  
  printf("addr of char is %p\n", p);
  printf("addr of char is %p\n", x);

  mem_free(x, 0);
  mem_dump();
  printf("%ld", sizeof(int));
}
