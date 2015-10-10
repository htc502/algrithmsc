#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algrithms.h"

node_t *addtree(node_t *p, char *w,int lvl)
{
  int cond;
  char *strdup_KR(char *s);
  node_t *talloc();

  if (p == NULL) {
    p = talloc();
    p->word = strdup_KR(w);
    p->count = 1;
    p->lvl=lvl;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w,p->word)) == 0)
    p->count++;
  else if (cond < 0)
    p->left = addtree(p->left,w,(p->lvl)+1);
  else
    p->right = addtree(p->right,w,(p->lvl)+1);
  /*  	printf("node add status:\n");
	printf("word: %s\n",p->word);
	printf("level: %d\n",p->lvl);
	printf("count: %d\n",p->count);
	printf("left: %s\n",((p->left == NULL) ? "NULL":p->left->word));
       	printf("right: %s\n",((p->right== NULL) ? "NULL":p->right->word));
       	printf("node status done\n");*/
  return p;

}
void treefree(node_t* root)
{
  struct node_t *left,*right;
  if((left=root->left) != NULL)
    treefree(left);
  if((right=root->right) != NULL)
    treefree(right);
  free(root->word);
  free(root);
}

void treeprint(node_t *p)
{
  if(p != NULL) 
    {
      treeprint(p->left);
      printf("%d %d %s\n",p->lvl,p->count,p->word);
      treeprint(p->right);
    }
}
char *strdup_KR(char *s)
{
  char *p;

  p = (char *)malloc(strlen(s)+1);
  if(p != NULL)
    strcpy(p,s);
  return p;
}
node_t *talloc(void)
{
  return (node_t*) malloc(sizeof(node_t));
}
/*getch_KR and ungetch_KR*/
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch_KR(void)
{
  return (bufp > 0) ? buf[--bufp]:getchar();
}
void ungetch_KR(int c)
{
  if (bufp >= BUFSIZE)
    printf("ungetch_KR: too many characters\n");
  else
    buf[bufp++] = c;
}

int getword(char *word,int lim)
{
  int c,getch_KR(void);
  void ungetch_KR(int);
  char *w = word;

  while (isspace(c = getch_KR()))
    ;
  if( c != EOF )
    *w++ = c;
  if(!(isalpha(c)||c == '_')) { /* alphabic or_ */
    *w='\0';
    return c;
  }
  for (; --lim > 0;w++)
    {
      *w = getch_KR();
      if(!(isalnum(*w)||*w=='_')) {
	ungetch_KR(*w);
	break;
      }
    }
  *w='\0';
  return word[0];

}
	
#define HASHSIZE 10
static nlist_t *hashtab[HASHSIZE];

unsigned hash(char *s)
{
  unsigned hashval;

  for(hashval=0;*s != '\0';s++)
    hashval = *s + 31*hashval;
  return hashval % HASHSIZE;
}

nlist_t *lookup(char *s)
{
  nlist_t *np;

  for(np=hashtab[hash(s)];np != NULL; np=np->next)
    if(strcmp(np->name,s) == 0)
      return np;
  return NULL;
}

nlist_t *install(char *name, char *defn)
{
  nlist_t *np;
  unsigned hashval;

  if((np=lookup(name)) == NULL) {
    np =(nlist_t *) malloc(sizeof(*np));
    if(np == NULL || (np->name = strdup_KR(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;
  } else
    free((void *) np->defn);
  if((np->defn = strdup_KR(defn)) == NULL)
    return NULL;
  return np;

}

/* release the entire nlist_t */
void rnlist(nlist_t *pn)
{
  if(pn == NULL)
    return;
  if(pn->next != NULL)
    rnlist(pn->next);
  free(pn->name);
  free(pn->defn);
  free(pn);
  pn=NULL;
}

/* release the hash table */
void releaseHS(){
  int i;
  for(i=0;i<HASHSIZE;i++){
    if(hashtab[i] != NULL) 
      rnlist(hashtab[i]);
  }
}

/* undef one nlist element */
nlist_t *undef(char *s)
{
  nlist_t *np,*np_pre;
  for(np=hashtab[hash(s)];np!=NULL;np=np->next){
    if(strcmp(s,np->name) == 0){
      np_pre->next = np->next;
      np->next=NULL;
      free(np->name);
      free(np->defn);
      free(np);
      return np_pre->next;
    }
    np_pre = np;/* record the parent node */
  }
  return NULL;
}

void printnlist(nlist_t* np)
{
  if(np == NULL)
    return;
  char *n="NULL";
  printf("(%s---%s)",(np->name != NULL? np->name:n),(np->defn != NULL? np->defn:n));
  nlist_t* mp;
  for(mp=np->next;mp!=NULL;mp=mp->next) 
    printf("->(%s---%s)",(mp->name!=NULL?mp->name:n),(mp->defn != NULL? mp->defn:n));
  return;
}

void printHS(){
  int i;
  for(i=0;i<HASHSIZE;i++){
    printnlist(hashtab[i]);
    printf("\n");
  }
  return;
}
