#include <stdio.h>

struct tnode {
  char *word;
  int count;
  int lvl;
  struct tnode *left;
  struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w,int lvl)
{
  int cond;
  char *strdup_KR(char *s);
  struct tnode *talloc();

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
void trdestry(struct tnode* root)
{
  struct tnode *left,*right;
  if((left=root->left) != NULL)
    trdestry(left);
  if((right=root->right) != NULL)
    trdestry(right);
  free(root->word);
  free(root);
}

void treeprint(struct tnode *p)
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
struct tnode *talloc(void)
{
  return (struct tnode*) malloc(sizeof(struct tnode));
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
	
struct nlist {
  struct nlist *next;
  char *name;
  char *defn;
};

#define HASHSIZE 10
static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s)
{
  unsigned hashval;

  for(hashval=0;*s != '\0';s++)
    hashval = *s + 31*hashval;
  return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
  struct nlist *np;

  for(np=hashtab[hash(s)];np != NULL; np=np->next)
    if(strcmp(np->name,s) == 0)
      return np;
  return NULL;
}

struct nlist *install(char *name, char *defn)
{
  struct nlist *np;
  unsigned hashval;

  if((np=lookup(name)) == NULL) {
    np =(struct nlist *) malloc(sizeof(*np));
    if(np == NULL || (np->name = strdup(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;
  } else
    free((void *) np->defn);
  if((np->defn = strdup(defn)) == NULL)
    return NULL;
  return np;

}

/* release the entire nlist */
void rnlist(struct nlist *pn)
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
struct nlist *undef(char *s)
{
  struct nlist *np,*np_pre;
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

void printnlist(struct nlist* np)
{
  if(np == NULL)
    return;
  char *n="NULL";
  printf("(%s---%s)",(np->name != NULL? np->name:n),(np->defn != NULL? np->defn:n));
  struct nlist* mp;
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
