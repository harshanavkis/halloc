#ifndef LIST_H
#define LIST_H

#include <limits.h>

typedef struct list
{
  int size; //free space available
  void *startaddr;
  struct list *next;
} list;

#endif
