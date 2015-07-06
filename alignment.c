/* an implementation of smith waterman local alignment algrithm */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

typedef enum {N, A, C, G, T} nt; //nucleotide
int W(int i) { return(-i);}
int S[5][5] = { {2,-1,-1,-1,-1},
		{-1,2,-1,-1,-1},
		{-1,-1,2,-1,-1},
		{-1,-1,-1,2,-1},
		{-1,-1,-1,-1,2} };
/* define a data structure to store the alignRes matrix and back trace */
typedef struct {
  int nrow;
  int ncol;
  int* array;
  int* bktrace; /* this is the back trace matr, each entry has a value of (-1,0,1) -1 for <-; 0 for <\; 1 for | */
} alignRes;

void alignResNew(alignRes* palignRes, int nrow, int ncol)
{
  assert(nrow > 0 && ncol > 0);
  palignRes->nrow = nrow;
  palignRes->ncol = ncol;
  palignRes -> array = (int*) malloc(nrow*ncol*sizeof(int));
  palignRes -> bktrace = (int*) malloc(nrow*ncol*sizeof(int));
  assert(palignRes->array != NULL);
  assert(palignRes->bktrace != NULL);
}

void alignResDispose(alignRes* palignRes)
{
  free(palignRes->array);
  free(palignRes->bktrace);
}

void setScore(alignRes* palignRes, int i, int j, int alignRes)
{
  assert(i >= 0 && i < palignRes->nrow);
  assert(j >= 0 && j < palignRes->ncol);

  *(palignRes->array + i*(palignRes->nrow) + j) = alignRes;
}

void setTrace(alignRes* palignRes, int i, int j, int trace)
{
  assert(i >= 0 && i < palignRes->nrow);
  assert(j >= 0 && j < palignRes->ncol);

  *(palignRes->bktrace + i*(palignRes->nrow) + j) = trace;
}

int getScore(alignRes* palignRes, int i, int j)
{
  assert(i >= 0 && i < palignRes->nrow);
  assert(j >= 0 && j < palignRes->ncol);

  return(*(palignRes->array + i*(palignRes->nrow) + j));
}

int getTrace(alignRes* palignRes, int i, int j)
{
  assert(i >= 0 && i < palignRes->nrow);
  assert(j >= 0 && j < palignRes->ncol);

  return(*(palignRes->bktrace + i*(palignRes->nrow) + j));
}

void showScore(alignRes* palignRes)
{
  int i,j;
  fprintf(stdout, "\n");
  for(i=0;i<palignRes->nrow;i++)
    {
      for(j=0;j<palignRes->ncol;j++)
	fprintf(stdout , "%2i ",getScore(palignRes, i,j));
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showTrace(alignRes* palignRes)
{
  int i,j;
  fprintf(stdout, "\n");
  for(i=0;i<palignRes->nrow;i++)
    {
      for(j=0;j<palignRes->ncol;j++)
	{
	  char tr;
	  switch(getTrace(palignRes, i, j))
	    {
	    case -1:
	      tr = '|';
	      break;
	    case 0:
	      tr = 92;
	      break;
	    case 1:
	      tr = '-';
	      break;
	    default:
	      tr = '?';
	    }
	  fprintf(stdout , "%2c ",tr);
	}
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showAlignment(alignRes* palignRes)
{
  /* get the largest value */

  /* trace back */
}

void sw(nt* pref,int reflen, //reference sequence and length
	nt* pseg, int seglen, alignRes* palignRes ) //segment sequence and length
{
  alignResNew(palignRes, reflen + 1, seglen + 1);

  int i = 0, j = 0;
  for(;i <= reflen; i++)
    setScore(palignRes, i ,0, 0);
  for(;j <= seglen; j++)
    setScore(palignRes, 0 ,j, 0);

  for(i = 1;i <= reflen; i++)
    {
      for(j = 1;j <= seglen; j++)
	{
	  int match =  getScore(palignRes, i-1, j-1) + S[*(pref + i - 1)][*(pseg + j - 1)];
	  int delete, insert;
	  int k = i,l = j, tmp = INT_MIN;
	  for(; k >= 1; k --)
	    tmp = getScore(palignRes,i-k,j) + W(k) > tmp ? getScore(palignRes,i-k,j)+W(k):tmp; //W() is gap penalty
	  delete = tmp;
	  tmp = INT_MIN;
	  for(; l >= 1; l--)
	    tmp = getScore(palignRes,i,j-l) + W(l) > tmp ? getScore(palignRes,i,j-l)+W(l):tmp;
	  insert = tmp;
	  int score = 0;
	  int trace = -1; /* delete */
	  score = match > delete ? match : delete;
	  if(score == match)
	    trace = 0;
	  score = score > insert ? score : insert;
	  if(score == insert)
	    trace = 1;
	  setScore(palignRes,i,j , score);
	  setTrace(palignRes,i,j , trace);
	}
    }
}

//let's test it first


int main()
{
  nt seq1[8] = {A,C,A,C,A,C,T,A};
  nt seq2[8] = {A,G,C,A,C,A,C,A};

  alignRes alignResobj;
  alignResNew(&alignResobj, 9,9);
  sw(seq1, 8,
     seq2, 8,
     &alignResobj);
  showScore(&alignResobj);
  showTrace(&alignResobj);
  alignResDispose(&alignResobj);
  return(0);    
}
