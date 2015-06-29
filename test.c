#include <stdio.h>
#include "algrithms.h"

int test_bs()
{
  char query[] = "aby";
  char *p[] = {"aby","bus","dog","focus","hear"};

  int res=  bs(query, p, 5);
  fprintf(stdout, "%i \n",res);
  return(0);
}

void printarray(int *array,int len)
{
  printf("[ ");
  int i=0;
  for(;i<len;i++)
    {
      printf("%d ",*(array+i));
    }
  printf("]\n");
}

void test_insetSort()
{
  int a[] = {0,3,5,2,4,2,1,45,2,1,1,4,4,6,7,8,4,3};
  insertSort(a, 18);
  printarray(a,18);
}

void test_ss()
{
  int a[] = {0,3,5,2,4,2,1,45,2,1,1,4,4,6,7,8,4,3};
  selectionSort(a,18);
  printarray(a,18);
}

void test_ms()
{
  int a[] = {0,3,5,2,4,2,1,45,2,1,1,4,4,6,7,8,4,3};
  mergeSort(a, 0, 17);
  printarray(a,18);
}

void test_hs()
{
  int a[] = {0,3,5,2,4,2,1,45,2,1,1,4,4,6,7,8,4,3};
  heapSort(a, 18);
  printarray(a,18);
}

void test_qs()
{
  int a[] = {0,3,5,2,4,2,1,45,2,1,1,4,4,6,7,8,4,3};
  quickSort(a, 0, 17);
  printarray(a,18);
}

void test_bwt()
{
  char x[]="this is a child who is eager to do something!";
  int pos[45],pos_s[45],i,m;
  bwt(x,45,pos);
  sortstring(x,45,pos_s);
  for(i=0;i<45;i++){
    m=pos[i];
    printf("%c",x[m]);
  }
  printf("\n");
  for(i=0;i<45;i++){
    m=pos_s[i];
    printf("%c",x[m]);
  }
  printf("\n");


}

void test_tree()
{
#define MAXWORD 100
#define HASHSIZE 10
  struct tnode *root;
  char word[MAXWORD];
  static struct nlist *hashtab[HASHSIZE];

  root=NULL;
  while (getword(word,MAXWORD) != EOF)
    {
      if (isalpha(word[0]))
	root = addtree(root,word,0);
    }
  treeprint(root);
  trdestry(root);
  int i=0;
  for(i=0;i<HASHSIZE;i++) {
    hashtab[i]=NULL;
  }
  char *den_default="defn_default";
  while (getword(word,MAXWORD) != EOF)
    {
      if (isalpha(word[0]))
	install(word,den_default);
    }
  printHS(hashtab,HASHSIZE);
  releaseHS();
}

int main()
{
  test_ms();
  test_ss();
  test_insetSort();
  test_bs();
  test_hs();
  test_qs();
  test_bwt();
  test_tree();
  return(0);
}
