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
  nt* ref; //the reference sequence
  nt* seg; //segment sequence
  int** score;
  int** btrace; /* this is the back trace matr, each entry has a value of (-1,0,1) -1 for <-; 0 for <\; 1 for | */
} Alignome; /* we call it alignome, the struct to record alignment result */

void setScore(Alignome* pAlignome, int idxref, int idxseg, int score);
void setTrace(Alignome* pAlignome, int idxref, int idxseg, int score);
void showTrace(Alignome* pAlignome);
void showScore(Alignome* pAlignome);

void AlignomeNew(Alignome* pAlignome,
		 int reflen, nt* ref,
		 int seglen, nt* seg)
{
  assert(reflen > 0 && seglen > 0);
  int idxref , idxseg ;
  pAlignome->reflen = reflen;
  pAlignome->seglen = seglen;
  pAlignome -> score = (int**) malloc((seglen+1)*sizeof(int*));
  for(idxseg=0;idxseg<=seglen;idxseg++)
      *(pAlignome->score + idxseg) = (int*) malloc((reflen + 1)*sizeof(int));
  pAlignome -> btrace = (int**) malloc((seglen+1)*sizeof(int*));
  for(idxseg=0;idxseg<=seglen;idxseg++)
      *(pAlignome->btrace + idxseg) = (int*) malloc((reflen + 1)*sizeof(int));
  pAlignome -> ref = (nt*) malloc(reflen*sizeof(nt));
  pAlignome -> seg = (nt*) malloc(seglen*sizeof(nt));
  assert(pAlignome->score && pAlignome->btrace && \
	 pAlignome->ref && pAlignome->seg);
  /* cpy seg and ref */
  for(idxref=0; idxref < reflen; idxref++)
    memcpy( pAlignome->ref + idxref, ref + idxref, sizeof(nt));
  for(idxseg=0; idxseg< seglen; idxseg++)
    memcpy( pAlignome->seg + idxseg, seg + idxseg, sizeof(nt));
  for(idxref=0;idxref<=reflen; idxref++)
    for(idxseg=0;idxseg<= seglen; idxseg++)
      setTrace(pAlignome,idxref,idxseg ,-2); /*anything other than -1,0,1 will be treated as unknown, here we chose -2 */
  /* set to INT_MIN excluding the first row and column */
  for(idxseg=1;idxseg<=seglen;idxseg++)
    for(idxref=1;idxref<=reflen;idxref++)
	setScore(pAlignome, idxref,idxseg,INT_MIN); 
}

void showNt(nt* pnt, int len)
{
  char alphabet[] = {'N','A','C','G','T'};
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
  int idxref,idxseg;
  for(idxseg=0;idxseg<=pAlignome->seglen;idxseg++)
    free(*(pAlignome->score + idxseg));
  free(pAlignome->score);
  for(idxseg=0;idxseg<=pAlignome->seglen;idxseg++)
    free(*(pAlignome->btrace + idxseg));
  free(pAlignome->btrace);
  free(pAlignome->ref);
  free(pAlignome->seg);
}

void setScore(Alignome* pAlignome, int idxref, int idxseg, int score)
{
  assert(idxref >= 0 && idxref <= pAlignome->reflen);
  assert(idxseg >= 0 && idxseg <= pAlignome->seglen);

  *(*(pAlignome->score + idxseg) + idxref) = score; /*2-d array indexing ,it's boring...;this is score[j,i] */
}

void setTrace(Alignome* pAlignome, int idxref, int idxseg, int score)
{
  assert(idxref >= 0 && idxref <= pAlignome->reflen);
  assert(idxseg >= 0 && idxseg <= pAlignome->seglen);

  *(*(pAlignome->btrace + idxseg) + idxref) = score; /*2-d array indexing ,it's boring...;this is score[j,i] */
}

int getScore(Alignome* pAlignome, int idxref, int idxseg)
{
  assert(idxref >= 0 && idxref <= pAlignome->reflen);
  assert(idxseg >= 0 && idxseg <= pAlignome->seglen);

  return(*(*(pAlignome->score + idxseg) +  idxref));
}

int getTrace(Alignome* pAlignome, int idxref, int idxseg)
{
  assert(idxref >= 0 && idxref <= pAlignome->reflen);
  assert(idxseg >= 0 && idxseg <= pAlignome->seglen);

  return(*(*(pAlignome->btrace+ idxseg) +  idxref));
}

void showScore(Alignome* pAlignome)
{
  int i,j;
  fprintf(stdout, "\n");
  for(j=0;j<=pAlignome->seglen;j++)
    {
      for(i=0;i<=pAlignome->reflen;i++)
	fprintf(stdout , "%5i ",getScore(pAlignome, i,j));
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showTrace0(Alignome* pAlignome)
{
  int i,j;
  fprintf(stdout, "\n");
  for(j=0;j<=pAlignome->seglen;j++)
    {
      for(i=0;i<=pAlignome->reflen;i++)
	{
	  int tr = getTrace(pAlignome, i, j);
	  fprintf(stdout , "%5i ",tr);
	}
      fprintf(stdout,"\n");
    }
  fprintf(stdout, "\n");
}

void showTrace(Alignome* pAlignome)
{
  int i,j;
  fprintf(stdout, "\n");
  for(j=0;j<=pAlignome->seglen;j++)
    {
      for(i=0;i<=pAlignome->reflen;i++)
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
	    case -2:
	      tr = 'x';
	      break;
	    default:
	      tr = '?';
	    }
	  fprintf(stdout , "%5c ",tr);
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

/* we need a backtrace fxn to recover the alignment */
/* void backtrace(Alignome *pAlignome)
   {
   int i,j; i = pAlignome.reflen; j = pAlignome.seglen;
   char *prefAlign, *psegAlign;
   int alignmentLen = getalignLen(); 
   prefAlign = malloc(alignmentLen*sizeof(char));
   psegAlign = malloc(alignmentLen*sizeof(char));
   while(i != 0 && j != 0)
   {
   char t = getTrace(pAlignome, i ,j);
   switch(t){
   case '|':
   case '-':
   case 92:
   case '?':
   default:
   }
   }

   ##(0,0)

   free(prefAlign);
   free(psegAlign);
   } */

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
  int idxref = 0, idxseg = 0;
  for(;idxref <= reflen; idxref++)
    setScore(pAlignome, idxref ,0, 0);
  for(;idxseg <= seglen; idxseg++)
    setScore(pAlignome, 0 ,idxseg, 0);

  for(idxref = 1;idxref <= reflen; idxref++)
    {
      for(idxseg = 1;idxseg <= seglen; idxseg++)
	{
	  int match =  getScore(pAlignome, idxref-1, idxseg-1) + S[*(pref + idxref - 1)][*(pseg + idxseg - 1)];
	  int delete, insert;
	  int k = idxref,l = idxseg, tmp = INT_MIN;
	  for(; k >= 1; k --)
	    tmp = getScore(pAlignome,idxref-k,idxseg) + W(k) > tmp ? getScore(pAlignome,idxref-k,idxseg)+W(k):tmp; //W() is gap penalty
	  delete = tmp;
	  tmp = INT_MIN;
	  for(; l >= 1; l--)
	    tmp = getScore(pAlignome,idxref,idxseg-l) + W(l) > tmp ? getScore(pAlignome,idxref,idxseg-l)+W(l):tmp;
	  insert = tmp;
	  int score = 0;
	  int trace = -1; /* delete */
	  score = match > delete ? match : delete;
	  if(score == match)
	    trace = 0;
	  score = score > insert ? score : insert;
	  if(score == insert)
	    trace = 1;
	  setScore(pAlignome,idxref,idxseg , score);
	  setTrace(pAlignome,idxref,idxseg , trace);
	}
    }
}

//let's test it first


int main()
{
  nt ref[6] = {A,C,A,C,A,C};
  nt seg[6] = {A,G,C,A,C,A};

  Alignome Alignomeobj;
  AlignomeNew(&Alignomeobj,
	      6,ref,
	      6,seg);
  showAlignome(&Alignomeobj);
  sw(&Alignomeobj);
  showAlignome(&Alignomeobj);
  AlignomeDispose(&Alignomeobj);
  return(0);    
}
