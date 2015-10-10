#ifndef HGC_STACK_H
#define HGC_STACK_H

typedef struct {
  void *elems;
  int elemSize;
  int loglen;
  int alloclen;
} hgc_stack_t;

void stackNew(hgc_stack_t* s, int elemSize);
void stackDispose(hgc_stack_t* s);
void stackPush(hgc_stack_t* s,
	       void* elemAddr);
void stackPop(hgc_stack_t* s,
	      void* elemAddr);

#endif
