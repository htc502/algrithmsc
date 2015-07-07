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
/* define a data structure to store the Alignome matrix and back trace */
typedef struct {
  int reflen;
  int seglen;
  nt* ref;
  nt* seg;
  int* score;
  int* btrace; /* this is the back trace matr, each entry has a value of (-1,0,1) -1 for <-; 0 for <\; 1 for | */
} Alignome; /* we call it alignome, the struct to record alignment result */

void AlignomeNew(Alignome* pAlignome,
		 int reflen, nt* ref,
		 int seglen, nt* seg)
{
  assert(reflen > 0 && seglen > 0);
  pAlignome->reflen = reflen;
  pAlignome->seglen = seglen;
  pAlignome -> score = (int*) malloc((reflen+1)*(seglen+1)*sizeof(int));
  pAlignome -> btrace = (int*) malloc((reflen+1)*(seglen+1)*sizeof(int));
  pAlignome -> ref = (nt*) malloc(reflen*sizeof(nt));
  pAlignome -> seg = (nt*) malloc(seglen*sizeof(nt));
  assert(pAlignome->score != NULL);
  assert(pAlignome->btrace != NULL);
  assert(pAlignome->ref != NULL);
  assert(pAlignome->seg != NULL);
  /* cpy seg and ref */
  int i = 0;
  for(; i < reflen; i++)
    memcpy( pAlignome->ref + i, ref + i, sizeof(nt));
  for(i=0; i < seglen; i++)
    memcpy( pAlignome->seg + i, seg + i, sizeof(nt));
    
}

void showNt(nt* pnt, int len)
{
  char alphabet[] = {'N','A','C','G','G','T'};
  int i = 0;
  for(; i < len; i++)
    fprintf(stdout, "%1c ",alphabet[ (int)(pnt[i]) ]);
  fprintf(stdout, "\n");
}

void showSeq(Alignome *pAlignome)
{
  showNt(pAlignome->ref, pAlignome->reflen);
  showNt(pAlignome->seg, pAlignome->seglen);
}

void AlignomeDispose(Alignome* pAlignome)
{
  free(pAlignome->score);
  free(pAlignome->btrace);
  free(pAlignome->ref);
  free(pAlignome->seg);
}

void setScore(Alignome* pAlignome, int i, int j, int score)
{
  assert(i >= 0 && i <= pAlignome->reflen);
  assert(j >= 0 && j <= pAlignome->seglen);

  *(pAlignome->score + i*(pAlignome->reflen) + j) = score;
}

void setTrace(Alignome* pAlignome, int i, int j, int trace)
{
  assert(i >= 0 && i <= pAlignome->reflen);
  assert(j >= 0 && j <= pAlignome->seglen);

  *(pAlignome->btrace + i*(pAlignome->reflen) + j) = trace;
}

int getScore(Alignome* pAlignome, int i, int j)
{
  assert(i >= 0 && i <= pAlignome->reflen);
  assert(j >= 0 && j <= pAlignome->seglen);

  return(*(pAlignome->score + i*(pAlignome->reflen) + j));
}

int getTrace(Alignome* pAlignome, int i, int j)
{
  assert(i >= 0 && i <= pAlignome->reflen);
  assert(j >= 0 && j <= pAlignome->seglen);

  return(*(pAlignome->btrace + i*(pAlignome->reflen) + j));
}

void showScore(Alignome* pAlignome)
{
  int i,j;
  fprintf(stdout, "\n");
  for(i=0;i<=pAlignome->reflen;i++)
    {
      for(j=0;j<=pAlignome->seglen;j++)
	fprintf(stdout , "%2i ",getScore(pAlignome, i,j));
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showTrace(Alignome* pAlignome)
{
  int i,j;
  fprintf(stdout, "\n");
  for(i=0;i<=pAlignome->reflen;i++)
    {
      for(j=0;j<=pAlignome->seglen;j++)
	{
	  char tr;
	  switch(getTrace(pAlignome, i, j))
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
	  fprintf(stdout , "%1c ",tr);
	}
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showAlignome(Alignome *pAlignome)
{
  fprintf(stdout, "=====Start Alignome=====\n");
  showSeq(pAlignome);
  fprintf(stdout, "------------------------\n");
  showScore(pAlignome);
  fprintf(stdout, "------------------------\n");
  showTrace(pAlignome);
  fprintf(stdout, "======End Alignome======\n");
}

void showAlignment(Alignome* pAlignome)
{
  /* get the largest value */
  
  /* trace back */
}

void sw( Alignome* pAlignome ) //segment sequence and length
{
  nt *pref, *pseg;
  int reflen, seglen;
  pref = pAlignome->ref;
  pseg = pAlignome->seg;
  reflen = pAlignome->reflen;
  seglen = pAlignome->seglen;
  int i = 0, j = 0;
  for(;i <= reflen; i++)
    setScore(pAlignome, i ,0, 0);
  for(;j <= seglen; j++)
    setScore(pAlignome, 0 ,j, 0);

  for(i = 1;i <= reflen; i++)
    {
      for(j = 1;j <= seglen; j++)
	{
	  int match =  getScore(pAlignome, i-1, j-1) + S[*(pref + i - 1)][*(pseg + j - 1)];
	  int delete, insert;
	  int k = i,l = j, tmp = INT_MIN;
	  for(; k >= 1; k --)
	    tmp = getScore(pAlignome,i-k,j) + W(k) > tmp ? getScore(pAlignome,i-k,j)+W(k):tmp; //W() is gap penalty
	  delete = tmp;
	  tmp = INT_MIN;
	  for(; l >= 1; l--)
	    tmp = getScore(pAlignome,i,j-l) + W(l) > tmp ? getScore(pAlignome,i,j-l)+W(l):tmp;
	  insert = tmp;
	  int score = 0;
	  int trace = -1; /* delete */
	  score = match > delete ? match : delete;
	  if(score == match)
	    trace = 0;
	  score = score > insert ? score : insert;
	  if(score == insert)
	    trace = 1;
	  setScore(pAlignome,i,j , score);
	  setTrace(pAlignome,i,j , trace);
	}
    }
}

//let's test it first


int main()
{
  nt seq1[8] = {A,C,A,C,A,C,T,A};
  nt seq2[8] = {A,G,C,A,C,A,C,A};

  Alignome Alignomeobj;
  AlignomeNew(&Alignomeobj,
	      8,seq1,
	      8,seq2);
  showAlignome(&Alignomeobj);
  sw(&Alignomeobj);
  showAlignome(&Alignomeobj);
  AlignomeDispose(&Alignomeobj);
  return(0);    
}
