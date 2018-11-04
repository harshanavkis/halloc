#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "mem.h"
#include "utils/list.h"

void *startmem = NULL;//location starting which memory is allocated for the user program
long reqsize   = 0;
enum errCode m_error;
int minit = 0;
list *head = NULL;
int numNodes = 50;
int bufNodes = 0;
void *dsmem = NULL;//starting location in memory where the data structures are stored, linked list in this case

int memdsinit(void)
{
  dsmem = startmem;
  int size = sizeof(list)*numNodes;
  startmem += size; //make space for the memory data structures
  reqsize  -= size; 
  return 0;
}

int mem_init(int size, int debug)
{
  long pagesize = sysconf(_SC_PAGESIZE);
  int numpages  = size/pagesize;

  if(debug==1);
  
  if(numpages*pagesize!=size)
    {
      if(size<numpages*pagesize);
      else
	numpages += 1;
    }

  printf("numpages=%d\n", numpages);
  reqsize = pagesize*numpages;

  //memory request
  int fd = open("/dev/zero", O_RDWR);
  startmem = mmap(NULL, reqsize, PROT_READ | PROT_WRITE, MAP_PRIVATE |MAP_ANONYMOUS, fd, 0);
  if (startmem == MAP_FAILED) { m_error = E_BAD_ARGS; return -1; }
  close(fd);

  //make the pages resident
  int i;
  char *temp = startmem; 
  for(i=0; i<numpages; i++)
    {
      char c = *temp;
      c++;
      temp += pagesize;
    }
  
  memdsinit();
  return 0;
}

void *mem_alloc(int size)
{
  void *memloc;
  if(head==NULL)
    {
      //header and requested space allocation
      header_t *temp_h = (header_t*)startmem;
      temp_h->size     = size;
      temp_h->magic    = 1234567;
      startmem         = startmem + sizeof(header_t);
      memloc           = startmem;
      startmem         = startmem + size;
      reqsize         -= (sizeof(header_t) + size);

      //data structure allocation
      head            = (list*)dsmem;
      dsmem          += sizeof(list);
      bufNodes        = 1;
      head->size      = reqsize;
      head->startaddr = startmem;
      head->next      = NULL;
      
      return memloc;
    }
  list *temp_l = head;
  while(temp_l->next != NULL)
    {
      if(temp_l->size >= size)
	break;
      temp_l = temp_l->next;
    }
  if(temp_l->size < size)
    {
      //not enough space
      m_error = E_NO_SPACE;
      return NULL;
    }
  //header and requested space allocation
  void *freestart  = temp_l->startaddr;
  header_t *temp_h = (header_t*)freestart;
  temp_h->size     = size;
  temp_h->magic    = 1234567;
  temp_l->size    -= sizeof(header_t);
  freestart       += sizeof(header_t);
  
  memloc = freestart;

  temp_l->startaddr = freestart + size;
  temp_l->size     -= size; 
  return memloc;
}

int mem_free(void *ptr, int coalesce)
{
  if(ptr == NULL)
    {
      return 0;
    }

  list * temp_l    = head;
  header_t *temp_h = ptr - (sizeof(header_t));
  void *hInfo = ptr + temp_h->size -1;
  
  void *th = temp_h;
  
  while(temp_l != NULL)
    {      
      if(hInfo == temp_l->startaddr-1)
	{
	  //freed block can fit in the beginning
	  temp_l->startaddr = hInfo-sizeof(header_t);
	  temp_l->size     += sizeof(header_t) + temp_h->size;
	  return 0;
	}
      else if(th >= temp_l->startaddr && hInfo <= temp_l->startaddr + temp_l->size -1)
	{
	  //attempt to free already freed block
	  printf("Error while freeing already freed block\n");
	  exit(1);
	}
      
      temp_l = temp_l->next;
    }

  //freed block present in between
  temp_l = (list*)dsmem;
  temp_l->startaddr = temp_h;
  temp_l->size      = sizeof(header_t) + temp_h->size;
  temp_l->next      = head;
  head              = temp_l;
  return 0;
}

void mem_dump()
{
  if(head == NULL)
    {
      printf("<------------Memory layout------------->\n");
      printf("Start location = %p, size = %ld\n", startmem, reqsize);
    }
  
  list *temp_h = head;
  printf("<------------Memory layout------------->\n");
  while(temp_h != NULL)
    {
      printf("Start location = %p, size = %d\n", temp_h->startaddr, temp_h->size);
      temp_h = temp_h->next;
    }
}
