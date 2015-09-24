#ifndef NEW_ALIGN_H
#define NEW_ALIGN_H

/* define an nucleotide type */
typedef enum {A,C,G,T} nt_t;
/* btrace type */
typedef enum {DELETE,INSERT,MATCH,TERMINATION,UNKNOWN} btrace_t;

/*we store the score&btrace symbol in a cell_t object, which is a matrix cell*/
typedef struct {
  int sH; /*for a horizontal move, deletion I think*/
  int sV; /* vertical move, insertion */
  int score;
  btrace_t btrace;
} cell_t;

/*t: A T T G C - T */
/*q: A T - G C T T */
/*q(uery) position 3 is a deletion with respect to t(emplate) while position 6 is a insertion accordingly  */

/* we put temp as columns and query as the rows in mtr*/
/* btrace for deletion will be -; | for insertion */
/* here is the logic for determining which one is deletion and which one is insertion:
   it is related with the temp and query position(temp-row, query-column or temp-column, query-row).
   here we assume that temp is in column and query is in row. we have the definition that the back trace
   sign is an indication of the alignment path in the dp matrix.
   For the alignment example shown above, we can use the number of sequence characters presented in the alignment
   matrix to describe it:
   (col)t: 1 2 3 4 5 5 6
   (row)q: 1 2 2 3 4 5 6
   as is known that, each path in dp matrix coresponds to an alignment, the thing shows above is the coordination of
   the best alignment. from this we can get the answer:
   for a deletion, row index does not change, but col index plus one,you can 
   imagine this will be a '-' in the dp matrix.
   For insertion, the situation is just the opposite, col stays but row changes,
   this is indicated by '|'.
*/

typedef cell_t** Matrix_t;

#define STACKSIZE 5000

typedef struct {
  char* pch;
  int len,pointer;
} stack_t;


int doAlign(nt_t* t, int, nt_t* q, int, int );
#endif
