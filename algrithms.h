#ifndef ALGRITHM_C_H
#define ALGRITHM_C_H

typedef struct node_t {
  char *word;
  int count;
  int lvl;
  struct node_t *left;
  struct node_t *right;
} node_t;

typedef struct nlist_t {
  struct nlist_t *next;
  char *name;
  char *defn;
} nlist_t;


int bs(char*, char**, int);
void insertSort(int*, int);
void selectionSort(int*, int);
void mergeSort(int*, int, int);
void heapSort(int*, int);
void quickSort(int*, int, int);
void bwt(char *str,int len,int *result);
void sortstring(char *str,int len,int *result);
node_t *addtree(node_t *p, char *w,int lvl);
void treefree(node_t* root);
void treeprint(node_t *p);
char *strdup_KR(char *s);
node_t *talloc(void);
void printHS();
void printnlist(nlist_t* np);
void releaseHS();
void rnlist(nlist_t *pn);
nlist_t *install(char *name, char *defn);
nlist_t *lookup(char *s);
unsigned hash(char *s);

#endif
