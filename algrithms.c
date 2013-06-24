/*
 * =====================================================================================
 *
 *       Filename:  algrithms.c
 *
 *    Description:  algrithms implemented in C
 *
 *        Version:  1.0
 *        Created:  06/18/2013 02:54:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ewre 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include<time.h>


#define MAXWORD 100

struct key {
	char *word;
	int count;
};

int binsearch(char *word, struct key tab[],int n)
{
	int cond;
	int low,high,mid;

	low = 0;
	high = n-1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

void insertSort(int* array,int n)
{
	int i=0,j=0,key=0;
	for(j=1;j<=n-1;j++)
	{
		key=*(array+j);
		i=j-1;
		while(i>=0 && *(array+i) > key)
		{
			*(array+i+1)=*(array+i);
			i-=1;
		}
		*(array+i+1)=key;
	}
}

void insertSort_reverse(int* array, int n)
{
	int i=0,k=0,key=0;
	for(i=1;i<n;i++)
	{
		key=array[i];
		k=i-1;
		while(key>array[k]&&k>=0)//maintain sorted numbers in decreasing order
		{
			array[k+1]=array[k];
			k-=1;
		}
		array[k+1]=key;
	}
}

void selectionSort(int* array,int n)
{
//loop invariant: everytime array[0:i] is sorted in desreasing order
	int i=0,temp=0,j=0,max_ind=0;
	for(i=0;i<n-1;i++)
	{
//find max index in i:n
		max_ind=i;
		for(j=i;j<n;j++)
		{
			if(array[j]>array[max_ind]) max_ind=j;
		}
//switch
		temp=array[max_ind];
		array[max_ind]=array[i];
		array[i]=temp;
	}
}
//todo
void merge(int* array,int n, int p,int q, int r)
{

}

void printarray(int *array,int n)
{
	printf("[ ");
	int i=0;
	for(i=0;i<n;i++)
	{
		printf("%d ",*(array+i));
	}
	printf("]\n");
}

void generatArray(int* parray,int n,int max)
{
	srand(time(NULL));
	int i=0;
	for(i=0;i<n;i++)
	{
		parray[i]=rand()%max;
	}
}

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
int trdestry(struct tnode* root)
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

#define MAXWORD 100
struct tnode *addtree(struct tnode *, char *,int);
void treeprint(struct tnode *);
int getword(char *, int);

int main()
{
	struct tnode *root;
	char word[MAXWORD];

	root=NULL;
/*	while (getword(word,MAXWORD) != EOF)
	{
		if (isalpha(word[0]))
			root = addtree(root,word,0);
	}
	treeprint(root);
	trdestry(root);*/
	/* initialize nlist array */
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

	return 0;

}
