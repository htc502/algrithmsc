/* code implementation note of programming paradigm from Jerry Cain, stanford */
/* by guangchun */
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void stackGrow(stack_t* s)
{
  s->alloclen *= 2;
  s->elems = realloc(s->elems,
		     s->alloclen * s->elemSize);
}

void stackNew(stack_t* s, int elemSize)
{
  assert(elemSize > 0);
  s->elemSize = elemSize;
  s->loglen = 0;
  s->alloclen = 4;
  s->elems = malloc( 4* elemSize);
  assert(s->elems != NULL);
}

void stackDispose(stack_t* s)
{
  free(s->elems);
}

void stackPush(stack_t* s, void* elemAddr)
{
  if(s->loglen == s->alloclen)
    stackGrow(s);
  //see generic coding trick bellow
  void* target = (char*) s->elems + s->loglen * s->elemSize;

  memcpy(target, elemAddr, s->elemSize);
  s->loglen++;
}

void stackPop(stack_t* s,
	      void* elemAddr)
{
  //generic coding trick comes again
  void* source = (char*) s-> elems + (s->loglen-1) * s->elemSize;

  memcpy(elemAddr, source, s->elemSize);
  s->loglen--;
}
