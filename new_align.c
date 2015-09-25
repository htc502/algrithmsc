/*think and draft out the vairables and functions before put down the code ....*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "new_align.h"

static int local = 1;/* global/local switch: 0-global;local otherwise*/

/*scoring system:
u -> matching score, 
sigma->mismatch penalty,
delta for indel penalty,also for gap extension,
p for gap opening penalty*/

#define u 2
#define sigma -5
static int S[4][4] = { {u,sigma,sigma,sigma},
		{sigma,u,sigma,sigma},
		{sigma,sigma,u,sigma},
		{sigma,sigma,sigma,u}};

#define delta -2 
#define p -10

/* character set */
static char alpha[] = {'A','C','G','T'};

/* global variable */
static nt_t* query, *temp;
static int qlen, tlen;
static Matrix_t M; 

int init(nt_t* q,int qlen, nt_t* t, int tlen,int parlocal);
void destroy();
void Score();
int backtrace();
void printscore();
void printbtrace();
int c2nt(char c);

/* all variables settle down, let's begin.. */

int doAlign(nt_t* t, int partlen, nt_t* q, int parqlen, int parlocal)
{
  if(-1 == init(t,partlen, q,parqlen, parlocal)) {
    destroy();
    return(-1);  
  };
  Score();
  if(-1 == backtrace()) {
    destroy();
    return(-1);  
  };
  destroy();
  return(0);  
}
int c2nt(char c)
{
  switch(c){
  case 'A':
    return 0;
  case 'C':
    return 1;
  case 'G':
    return 2;
  case 'T':
    return 3;
  case 'a':
    return 0;
  case 'c':
    return 1;
  case 'g':
    return 2;
  case 't':
    return 3;
  }
  return -1;
}

int init(nt_t* q,int qlen0, nt_t* t, int tlen0, int parlocal) /* init everything */
{
  qlen = qlen0; tlen = tlen0;
  query = (nt_t*)malloc(qlen*sizeof(nt_t));
  if(!query)
    return(-1);
  temp = (nt_t*)malloc(tlen*sizeof(nt_t));
  if(!temp)
    return(-1);
  int idx;
  for(idx=0;idx<qlen;idx++)
    assert(memcpy(query+idx, q+idx, sizeof(nt_t)));
  for(idx=0;idx<tlen;idx++)
    assert(memcpy(temp+idx , t+idx, sizeof(nt_t)));
  M = (cell_t**)malloc((qlen+1)*sizeof(cell_t*));
  if(!M)
    return(-1);
  for(idx=0;idx<=qlen;idx++)
    M[idx] = NULL;
  for(idx=0;idx<=qlen;idx++) {
    M[idx] = (cell_t*)malloc((tlen+1)*sizeof(cell_t));
    if(!M[idx])
      return(-1);
    int idy;
    for(idy=0;idy<=tlen;idy++) {
      M[idx][idy].score = INT_MIN;
      M[idx][idy].sV= INT_MIN;
      M[idx][idy].sH= INT_MIN;
      M[idx][idy].btrace= UNKNOWN;
    }
  }

  /* init first row and col */
  M[0][0].score = 0;
  M[0][0].sV = 0;
  M[0][0].sH = 0;
  M[0][0].btrace = TERMINATION;
  int irow,icol;
  for(irow=1;irow<=qlen;irow++) {
    M[irow][0].btrace = INSERT;
    M[irow][0].score = delta * irow;
    M[irow][0].sH = M[irow][0].score;
    M[irow][0].sV = INT_MIN; /* m[irow][0].sV should be NA, because there's no way to move with | or \ to get here*/
  }
  for(icol=1;icol<=tlen;icol++) {
    M[0][icol].btrace = DELETE;
    M[0][icol].score = delta * icol;
    M[0][icol].sV = M[0][icol].score;
    M[0][icol].sH = INT_MIN; /* see reason above*/
  }
  local = parlocal;
  return(0);
}

int max(int a, int b, int c)
{
  int tmp = a >= b? a:b; 
  return tmp >= c ? tmp:c;
}
int which(int a, int b,int c)
{
  int mx = max(a,b,c);
  if(mx == a)
    return(1);
  if(mx == b)
    return(2);
  if(mx == c)
    return(3);
  return(-1);
}

void Score()
{
  int irow, icol;
  for(irow = 1;irow <= qlen;irow++)
    for(icol = 1;icol <= tlen;icol++)
      {
	int match, insert_init,insert_ext,  del_init, del_ext;
	match = M[irow-1][icol-1].score + S[ query[irow-1] ][ temp[icol-1] ];
	del_init = M[irow][icol-1].score + p + delta;
	del_ext = M[irow][icol-1].sH + delta;
	M[irow][icol].sH = del_init >= del_ext ? del_init : del_ext;
	insert_init  = M[irow-1][icol].score + p + delta;
	insert_ext = M[irow-1][icol].sV + delta;
	M[irow][icol].sV = insert_init >= insert_ext ? insert_init : insert_ext;
	int tmpMax = max(match,M[irow][icol].sV,M[irow][icol].sH);
	int wh = which(match,M[irow][icol].sV,M[irow][icol].sH);
	if(local && 0 >= tmpMax) {/* only in this situation we need termination...*/
	  M[irow][icol].score = 0;
	  M[irow][icol].btrace = TERMINATION;
	}
	else {
	  M[irow][icol].score = tmpMax;
	  switch(wh) {
	  case 1:
	    M[irow][icol].btrace = MATCH;
	    break;
	  case 2:
	    M[irow][icol].btrace = INSERT;
	    break;
	  case 3:
	    M[irow][icol].btrace = DELETE;
	    break;
	  default:
	    M[irow][icol].btrace = UNKNOWN;
	  }
	}
      }
}

int initstack(stack_t *ps)
{
  ps->pch=malloc(STACKSIZE*sizeof(char));
  if(!ps->pch)
    return(-1);
  int idx;
  for(idx=0;idx<STACKSIZE;idx++)
    ps->pch[idx] = '?';
  ps->len=STACKSIZE;
  ps->pointer = 0;
  return(0);
}
void freestack(stack_t *ps)
{
  free(ps->pch);
}
void printstack(stack_t *ps)
{
  int idx;
  for(idx=ps->pointer-1;idx>=0;idx--)
    fprintf(stdout,"%c ",(ps->pch)[idx]);
  fprintf(stdout,"\n");
}

int push(stack_t* ps,char ch)
{
  if(ps->pointer == (ps->len-1) ) {
    ps->pch = (char*)realloc(ps->pch, 2*(ps->len)*sizeof(char));
    if(!ps->pch)
      return(-1);
    else
      ps->len *= 2;
  }
  assert(memcpy(ps->pch+((ps->pointer)++), &ch,sizeof(char)));
  return(0);
}
int backtrace()
{
  void bestlocal(int*, int*);
  int irow,icol;
  irow = qlen;icol = tlen;
  if(local) 
    bestlocal(&irow,&icol);
  stack_t q,t;
  if(-1 == initstack(&q) || -1==initstack(&t))
    return(-1);
  while(irow != 0 || icol != 0) {
    switch(M[irow][icol].btrace) {
    case MATCH:
      push(&t,alpha[temp[--icol]]);
      push(&q,alpha[query[--irow]]);
      break;
    case INSERT:
      push(&t,'-');
      push(&q,alpha[query[--irow]]);
      break;
    case DELETE:
      push(&q,'-');
      push(&t,alpha[temp[--icol]]);
      break;
    case TERMINATION: /*we have to stop */
      irow = 0;
      icol = 0;
      break;
    case UNKNOWN:
      fprintf(stderr,"%i %i has unknown btrace sign,will terminate\n",
	      irow,icol);
      irow = 0;
      icol = 0;
      break;
    default:
      fprintf(stderr,"at[%i,%i], something strange happened\n",
	      irow,icol);
    }
  }
  fprintf(stdout,">>>>>>>>>Alignment:%i>>>>>>>>>>>>\n",M[qlen][tlen].score);
  printstack(&t);
  printstack(&q);
  fprintf(stdout,"<<<<<<<<<Alignment<<<<<<<<<<<<<<<\n");
  freestack(&q);freestack(&t);
  return(0);
}

void bestlocal(int* irow, int* icol)
{
  int idx,idy;
  int m = INT_MIN;
  for(idx=1;idx<=qlen;idx++)
    for(idy=1;idy<=tlen;idy++) {
      if(M[idx][idy].score > m) {
	*irow = idx;
	*icol = idy;
	m = M[idx][idy].score;
      }
    }
}

void printscore()
{
  int irow,icol;
  fprintf(stdout, "------------score(nrow:%i-ncol:%i)------------\n",qlen,tlen);
  for(irow = 0;irow <= qlen;irow++){
    for(icol = 0;icol <= tlen;icol++)
      fprintf(stdout,"%2i ",M[irow][icol].score);
    fprintf(stdout,"\n");
  }
}
void printbtrace()
{
  int irow,icol;
  fprintf(stdout, "------------btrace(nrow:%i-ncol:%i)------------\n",qlen,tlen);
  for(irow = 0;irow <= qlen;irow++){
    for(icol = 0;icol <= tlen;icol++)
      fprintf(stdout,"%2i ",M[irow][icol].btrace);
    fprintf(stdout,"\n");
  }
}
void destroy()
{
  free(query);free(temp);
  int idx;
  for(idx=0;idx<=qlen;idx++)
    free(M[idx]);
  free(M);
}
