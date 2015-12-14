/*think and draft out the vairables and functions before put down the code ....*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <unistd.h>
#include "hgc_stack.h"
#include "new_align.h"
#include "ini.h"

#define MAXLEN 2000
#define MY_MIN (INT_MIN/2)

static int local = 1 ;/* global/local switch: 0-global;local otherwise*/

/*scoring system:
  u -> matching score, 
  sigma->mismatch penalty,
  delta for also for gap extension,
  p for gap opening penalty*/

static float u;
static float delta;
static float p;
static float sigma;
static float S[4][4];

/* character set */
static char alpha[] = {'A','C','G','T'};

/* global variable */
static nt_t* query, *temp;
static int qlen, tlen;
static Matrix_t M; 

void scoringSys_init();
int init(nt_t* q,int qlen, nt_t* t, int tlen);
void destroy();
void Score();
int backtrace();
int c2nt(char c);

/* all variables settle down, let's begin.. */

int doAlign(nt_t* t, int partlen, nt_t* q, int parqlen)
{
  void scoringSys_init();
  scoringSys_init();
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


void scoringSys_init()
{
  /* default scoring system */
  u = 5;delta = -1;p = -10; sigma = -4;
  int irow,icol,idx;
  for(irow=0;irow < 4;irow++)
    for(icol=0;icol< 4;icol++)
      S[irow][icol] = sigma;
  for(idx=0;idx<4;idx++)
    S[idx][idx] = u;

  char* basenm= "config.ini"; /* scoring system can be redefined with this file */
  char* fini=(char*) malloc(sizeof(char)*MAXLEN);
  fini = getcwd(fini,MAXLEN);
  if(fini == NULL) {
    fprintf(stderr, "problem loading config.ini(file path too long?),use default\n");
    free(fini);
    return;
  }
  strcat(fini,"/");
  strcat(fini, basenm); 
  config_t config; 
  if(ini_parse(fini, handler, &config) < 0) 
    fprintf(stderr, "problem loading config.ini,use default\n");
  else {
    local = config.local;
    u = config.u; delta = config.delta; p = config.p; sigma = config.sigma;
    int irow,icol,idx;
    for(irow=0;irow < 4;irow++)
      for(icol=0;icol< 4;icol++)
	S[irow][icol] = sigma;
    for(idx=0;idx<4;idx++)
      S[idx][idx] = u;
  }
  free(fini);
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
      M[idx][idy].score = MY_MIN;
      M[idx][idy].sV= MY_MIN;
      M[idx][idy].sH= MY_MIN;
      M[idx][idy].btrace= UNKNOWN;
    }
  }

  /* init first row and col */
  M[0][0].score = 0;
  M[0][0].sV = 0;
  M[0][0].sH = 0;
  M[0][0].btrace = TERMINATION;
  int irow,icol;
  float insert, insert_init, insert_ext, del, del_init,del_ext;
  float s_hijk = M[0][0].score + 0; /* see intr of s_hijk in Score function */
  float max2(float,float);
  int which_max2(float,float);
  for(irow=1;irow<=qlen;irow++) {
    insert_init = M[irow-1][0].score + p + delta;
    insert_ext = M[irow-1][0].sV + delta;
    insert = max2(insert_init, insert_ext);
    M[irow][0].sV = insert;
    M[irow][0].sH = MY_MIN; /* m[irow][0].sV should be NA, because there's no way to move there with - or \ */
    if(local) {
      float tmpMax = max2(insert, s_hijk);
      M[irow][0].score = tmpMax;
      int wh = which_max2(insert, s_hijk);
      switch(wh) {
      case 1:
	M[irow][0].btrace = INSERT;
	break;
      case 2:
	M[irow][0].btrace = TERMINATION;
	break;
      default:
	M[irow][0].btrace = UNKNOWN;
      }
    }
    else {
      M[irow][0].score = insert;
      M[irow][0].btrace = INSERT;
    }
  }
  for(icol=1;icol<=tlen;icol++) {
    del_init = M[0][icol-1].score + p + delta;
    del_ext = M[0][icol-1].sH + delta;
    del = max2(del_init, del_ext);
    M[0][icol].sH = del;
    M[0][icol].sV = MY_MIN; /* see reason above*/
    if(local) {
      int tmpMax = max2(del, s_hijk);
      M[0][icol].score = tmpMax;
      int wh = which_max2(del, s_hijk);
      switch(wh) {
      case 1:
	M[0][icol].btrace = DELETE;
	break;
      case 2:
	M[0][icol].btrace = TERMINATION;
	break;
      default:
	M[0][icol].btrace = UNKNOWN;
      }
    }
    else {
      M[0][icol].btrace = DELETE;
      M[0][icol].score = del;
    }
  }
  return(0);
}
float max2(float a, float b)
{
  return a >= b ? a:b;
}
int which_max2(float a, float b)
{
  float mx = max2(a,b);
  if(mx == a)
    return(1);
  if(mx == b)
    return(2);
  return(-1);
}
float max3(float a, float b, float c)
{
  float tmp = a >= b? a:b;
  return tmp >= c ? tmp:c;
}
int which_max3(float a, float b,float c)
{
  float mx = max3(a,b,c);
  if(mx == a)
    return(1);
  if(mx == b)
    return(2);
  if(mx == c)
    return(3);
  return(-1);
}
float max4(float a, float b, float c ,float d)
{
  float tmp1, tmp2;
  tmp1 = max2(a,b);
  tmp2 = max2(c,d);
  return tmp1 >= tmp2 ? tmp1:tmp2;
}
int which_max4(float a, float b,float c,float d)
{
  float mx = max4(a,b,c,d);
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
	float match,insert, insert_init,insert_ext,del,del_init, del_ext;
	match = M[irow-1][icol-1].score + S[ query[irow-1] ][ temp[icol-1] ];
	del_init = M[irow][icol-1].score + p + delta;
	del_ext = M[irow][icol-1].sH + delta;
	del = max2(del_init, del_ext);
	M[irow][icol].sH = del;
	insert_init  = M[irow-1][icol].score + p + delta;
	insert_ext = M[irow-1][icol].sV + delta;
	insert = max2(insert_init, insert_ext);
	M[irow][icol].sV = insert;
	float s_hijk = M[0][0].score + 0; /* s_hijk for source hijack, see AIBA p183, this enables the local alignment */
	if(local) {
	  float tmpMax = max4(match, insert, del, s_hijk);
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
	  float tmpMax = max3(match,insert,del);
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
  fprintf(stdout,">>>>>>>>>AlignScore:%f>>>>>>>>>>>>\n",M[qstop][tstop].score);
  if(local)
    fprintf(stdout,"%d-%d\n",tstop-t.n_nt + 1,tstop);
  printstack(&t);
  fprintf(stdout,"\n");
  printstack(&q);
  fprintf(stdout,"\n");
  if(local)
    fprintf(stdout,"%d-%d\n",qstop-q.n_nt + 1,qstop);
  fprintf(stdout,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  stackDispose(&q);stackDispose(&t);
  return(0);
} 

void bestlocal(int* irow, int* icol)
{
  int idx,idy;
  float m = MY_MIN;
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
