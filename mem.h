#ifndef MEM_H
#define MEM_H


enum errCode
  {
    E_BAD_ARGS,
    E_NO_SPACE,
  };

typedef struct __header_t
{
  int size; //size of allocated block
  int magic; //integrity check
} header_t;

int memdsinit(void);
int mem_init(int size, int debug);
void *mem_alloc(int size);
//int mem_free(void* ptr, int coalesce);
//void mem_dump();

#endif
