#ifndef STACK_H
#define STACK_H

typedef struct {
  void *elems;
  int elemSize;
  int loglen;
  int alloclen;
} stack;

void stackNew(stack* s, int elemSize);
void stackDispose(stack* s);
void stackPush(stack* s,
	       void* elemAddr);
void stackPop(stack* s,
	      void* elemAddr);

#endif
