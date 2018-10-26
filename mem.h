#ifndef MEM_H
#define MEM_H


enum errCode
  {
    E_BAD_ARGS,
    E_NO_SPACE,
  };

int mem_init(int size, int debug);
void *mem_alloc(int size);
int mem_free(void* ptr, int coalesce);
void mem_dump();

#endif
