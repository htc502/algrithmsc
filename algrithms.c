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
#include <time.h>
#include <limits.h>


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

/*one word for insertsort: insertsort simulates the work u have done when u play with poke cards */

void insertSort(int* array,int n)
{
	int i=0,j=0,key=0;
	for(j=1;j<n;j++)
	{
		key=*(array+j);
		i=j-1;
		while(i>=0 && *(array+i) > key)/* loop from j-1 to 0 */
		{
			*(array+i+1)=*(array+i);
			i-=1;
		}
		*(array+i+1)=key;
	}
}

void insertSort_reverse(int* array, int n)
{
	int i=0,j=0,key=0;
	for(j=1;j<n;j++)
	{
		key=array[j];
		i=j-1;
		while(key>array[i]&&i>=0)//maintain sorted numbers in decreasing order
		{
			array[i+1]=array[i];
			i-=1;
		}
		array[i+1]=key;/* i has been minused by 1, so add it back */
	}
}

void selectionSort(int* array,int n)
{
//loop invariant: everytime array[0:i] is sorted in desreasing order
	int i=0,temp=0,j=0,max_ind=0;
	for(i=0;i<n;i++)
	{
//find index of the max element in i+1:n-1
		max_ind=i+1;
		for(j=i+1;j<n;j++)
		{
			if(array[j]>array[max_ind]) max_ind=j;
		}
//switch
		temp=array[max_ind];
		array[max_ind]=array[i+1];
		array[i+1]=temp;
	}
}
//mergeSort
void merge(int* array,int len, int p,int q, int r)
{
	//p,q,r are all start from zero
	void printarray(int*,int,int,int);
	int L[q-p+1+1], R[r-q+1];//one more cell for the sentinel value
	//when dealing with sorted sequence combine problem, sentinel value is a simple and good solution
	int i=0;
	for(i=p;i<=r;i++)
		i>q ? (R[i-q-1]=array[i]):(L[i-p]=array[i]);
	L[q-p+1]=INT_MAX;
	R[r-q]=INT_MAX;
	int li=0,ri=0,x=p;
	while((L[li]!= INT_MAX) ||(R[ri]!=INT_MAX)) {//for(m=p,m<=r,m++) {... array[m]=L[?]/R[?] ...}
		if(L[li] <= R[ri]){
			array[x++]=L[li++];
		}
		else{
			array[x++]=R[ri++];
		}
	}	
}
void mergeSort(int *array, int start, int end)
{
	void merge(int *,int,int,int,int);
	void printarray(int *,int,int,int);
	if(end > start){
		int split=(end+start)/2;
		mergeSort(array,start,split);
		mergeSort(array,split+1,end);
		merge(array,(end-start+1),start,split,end);
	}
}

void printarray(int *array,int len,int start,int end)
{
	if(start<0 || (end-start+1) > len){
		printf("index out of range\n");
		return;
	}
	printf("[ ");
	int i=0;
	for(i=start;i<=end;i++)
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


/* another form of Burrow Wheller Transform */
//printable characters
char prtchr[]={32,33,34,35,36,37,38,39,40,41, \
		42,43,44,45,46,47,48,49,50,51, \
		52,53,54,55,56,57,58,59,60,61, \
		62,63,64,65,66,67,68,69,70,71, \
		72,73,74,75,76,77,78,79,80,81, \
		82,83,84,85,86,87,88,89,90,91, \
		92,93,94,95,96,97,98,99,100,101, \
		101,103,104,105,106,107,108,109,110, \
		111,112,113,114,115,116,117,118,119, \
		120,121,122,123,124,125,126};//char a='x'is equivalent to char a=120, but char a='120' is nothing but a fault!

void bwt(char *str,int len,int *result)
{
	int prtchrNum=strlen(prtchr);
	int i=0,j=0,x=0;
	for(i=0;i<prtchrNum;i++){
		char ch=prtchr[i];
		for(j=0;j<len;j++){
			if(str[j] == ch)
				result[x++]=(j==(len-1)?0:(j+1));//j-1 is ok
		}
	}
}
void sortstring(char *str,int len,int *result)
{
	int prtchrNum=strlen(prtchr);
	int i=0,j=0,x=0;
	for(i=0;i<prtchrNum;i++){
		char ch=prtchr[i];
		for(j=0;j<len;j++){
			if(str[j] == ch)
				result[x++]=j;
		}
	}
}
#define MAXWORD 100
struct tnode *addtree(struct tnode *, char *,int);
void treeprint(struct tnode *);
int getword(char *, int);

int main()
{
	/* struct tnode *root;
	char word[MAXWORD];

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
	releaseHS();*/
//	int x[13]={4,7,9,10,12,30,7,9,10,14,16,18,29};
	/*int x[10];
	generatArray(x,10,19);
	printarray(x,10,0,9);
	mergeSort(x,0,9);
	//merge(x,13,0,5,12);
	printarray(x,10,0,9);*/

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
	return 0;

}
