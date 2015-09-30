#ifndef STACK_H
#define STACK_H

typedef struct {
  void *elems;
  int elemSize;
  int loglen;
  int alloclen;
} stack_t;

void stackNew(stack_t* s, int elemSize);
void stackDispose(stack_t* s);
void stackPush(stack_t* s,
	       void* elemAddr);
void stackPop(stack_t* s,
	      void* elemAddr);

#endif
