#ifndef ALGRITHM_C_H
#define ALGRITHM_C_H

int bs(char*, char**, int);
void insertSort(int*, int);
void selectionSort(int*, int);
void mergeSort(int*, int, int);
void heapSort(int*, int);
void quickSort(int*, int, int);
void bwt(char *str,int len,int *result);
void sortstring(char *str,int len,int *result);
struct tnode *addtree(struct tnode *p, char *w,int lvl);
void trdestry(struct tnode* root);
void treeprint(struct tnode *p);
char *strdup_KR(char *s);
struct tnode *talloc(void);
void printHS();
void printnlist(struct nlist* np);
void releaseHS();
void rnlist(struct nlist *pn);
struct nlist *install(char *name, char *defn);
struct nlist *lookup(char *s);
unsigned hash(char *s);

#endif
