/*think and draft out the vairables and functions before put down the code ....*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "hgc_stack.h"
#include "new_align.h"
#include "ini.h"

static int local = 1;/* global/local switch: 0-global;local otherwise*/

/*scoring system:
  u -> matching score, 
  sigma->mismatch penalty,
  delta for indel penalty,also for gap extension,
  p for gap opening penalty*/
char* fini = "/home/ewre/Projects/algrithmsc/config.ini"; /* scoring system can be redefined with this file */

static int u = 2;
static int delta = -2;
static int p = -10;
static int sigma = -5;
/* static int S[4][4] = { {u,sigma,sigma,sigma},
   {sigma,u,sigma,sigma},
   {sigma,sigma,u,sigma},
   {sigma,sigma,sigma,u}}; */

static int S[4][4];
/* character set */
static char alpha[] = {'A','C','G','T'};

/* global variable */
static nt_t* query, *temp;
static int qlen, tlen;
static Matrix_t M; 

void read_config();
int init(nt_t* q,int qlen, nt_t* t, int tlen);
void destroy();
void Score();
int backtrace();
int c2nt(char c);

/* all variables settle down, let's begin.. */

int doAlign(nt_t* t, int partlen, nt_t* q, int parqlen)
{
  void read_config();
  read_config();
  if(-1 == init(t,partlen, q,parqlen)) {
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


typedef struct {
  int local;
  int p, u, sigma, delta;
} config_t;

#define MATCH(s,n) strcmp(section,s) == 0 && strcmp(name,n) ==0
static int handler(void* pconf,
		   const char* section,
		   const char* name,
		   const char* value)
{
  config_t* p = (config_t*) pconf;
  if(MATCH("type", "local"))
    p->local = atoi(value);
  else if(MATCH("scoring", "u"))
    p->u = atoi(value);
  else if(MATCH("scoring", "delta"))
    p->delta = atoi(value);
  else if(MATCH("scoring", "p"))
    p->p = atoi(value);
  else if(MATCH("scoring", "sigma"))
    p->sigma = atoi(value);
  else
    return 0;/* unknown name/section error */
  return 1;
}


void read_config()
{
  config_t config; 
  if(ini_parse(fini, handler, &config) < 0)
    fprintf(stderr, "problem loading config.ini,use default\n");
  else {
    local = config.local;
    u = config.u; delta = config.delta; p = config.p; sigma = config.sigma;
  }
  int irow,icol,idx;
  for(irow=0;irow < 4;irow++)
    for(icol=0;icol< 4;icol++)
      S[irow][icol] = sigma;
  for(idx=0;idx<4;idx++)
    S[idx][idx] = u;
}

int init(nt_t* q,int qlen0, nt_t* t, int tlen0) /* init sequence and alignment matrix */
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
  return(0);
}
int max2(int a, int b)
{
  return a >= b ? a:b;
}
int max3(int a, int b, int c)
{
  int tmp = a >= b? a:b; 
  return tmp >= c ? tmp:c;
}
int which_max3(int a, int b,int c)
{
  int mx = max3(a,b,c);
  if(mx == a)
    return(1);
  if(mx == b)
    return(2);
  if(mx == c)
    return(3);
  return(-1);
}
int max4(int a, int b, int c ,int d)
{
  int tmp1, tmp2;
  tmp1 = max2(a,b);
  tmp2 = max2(c,d);
  return tmp1 >= tmp2 ? tmp1:tmp2;
}
int which_max4(int a, int b,int c,int d)
{
  int mx = max4(a,b,c,d);
  if(mx == a)
    return(1);
  if(mx == b)
    return(2);
  if(mx == c)
    return(3);
  if(mx == d)
    return(4);
  return(-1);
}
void Score()
{
  int irow, icol;
  for(irow = 1;irow <= qlen;irow++)
    for(icol = 1;icol <= tlen;icol++)
      {
	int match,insert, insert_init,insert_ext,del,del_init, del_ext;
	match = M[irow-1][icol-1].score + S[ query[irow-1] ][ temp[icol-1] ];
	del_init = M[irow][icol-1].score + p + delta;
	del_ext = M[irow][icol-1].sH + delta;
	del = max2(del_init, del_ext);
	M[irow][icol].sH = del;
	insert_init  = M[irow-1][icol].score + p + delta;
	insert_ext = M[irow-1][icol].sV + delta;
	insert = max2(insert_init, insert_ext);
	M[irow][icol].sV = insert;
	int s_hijk = M[0][0].score + 0; /* s_hijk for source hijack, see AIBA p183, this enables the local alignment */
	if(local) {
	  int tmpMax = max4(match, insert, del, s_hijk);
	  M[irow][icol].score = tmpMax;
	  int wh = which_max4(match, insert, del, s_hijk);
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
	  case 4:
	    M[irow][icol].btrace = TERMINATION;
	    break;
	  default:
	    M[irow][icol].btrace = UNKNOWN;
	  }
	}
	else {
	  int tmpMax = max3(match,insert,del);
	  int wh = which_max3(match,insert,del);
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

void printstack(hgc_stack_t *ps)
{
  int idx;
  for(idx=ps->loglen-1;idx>=0;idx--)
    fprintf(stdout,"%c ",*((char*)(ps->elems + idx*ps->elemSize))); /* we know that its a char stack */
}

int backtrace()
{
  void bestlocal(int*, int*);
  int irow,icol;
  int qstop,tstop; /* qstop,tstop for print local alignment */
  qstop = irow = qlen;tstop = icol = tlen;

  if(local) { 
    bestlocal(&irow,&icol);
    qstop=irow;tstop=icol;
  }
  hgc_stack_t q,t;
  stackNew(&q,sizeof(char));stackNew(&t,sizeof(char));

  char tmp, ext;
  ext = '-';
  while(irow != 0 || icol != 0) {
    switch(M[irow][icol].btrace) {
    case MATCH:
      tmp = alpha[temp[--icol]];
      stackPush(&t,&tmp);
      tmp = alpha[query[--irow]];
      stackPush(&q,&tmp);
      break;
    case INSERT:
      stackPush(&t,&ext);
      tmp = alpha[query[--irow]];
      stackPush(&q,&tmp);
      break;
    case DELETE:
      stackPush(&q,&ext);
      tmp = alpha[temp[--icol]];
      stackPush(&t,&tmp);
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
  if(local)
    fprintf(stdout,"%d-%d\n",tstop-t.loglen + 1,tstop);
  printstack(&t);
  fprintf(stdout,"\n");
  printstack(&q);
  fprintf(stdout,"\n");
  if(local)
    fprintf(stdout,"%d-%d\n",qstop-q.loglen + 1,qstop);
  fprintf(stdout,"<<<<<<<<<Alignment<<<<<<<<<<<<<<<\n");
  stackDispose(&q);stackDispose(&t);
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

void destroy()
{
  free(query);free(temp);
  int idx;
  for(idx=0;idx<=qlen;idx++)
    free(M[idx]);
  free(M);
}
