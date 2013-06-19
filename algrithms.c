/*
 * =====================================================================================
 *
 *       Filename:  structure.c
 *
 *    Description:  algrithms described in K&R the C programming language chapter6
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
	struct tnode *left;
	struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;
	char *strdup_KR(char *s);
	struct tnode *talloc();

	if (p == NULL) {
		p = talloc();
		p->word = strdup_KR(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w,p->word)) == 0)
		p->count++;
	else if (cond < 0)
		p->left = addtree(p->left,w);
	else
		p->right = addtree(p->right,w);
	return p;

}
void treeprint(struct tnode *p)
{
	if(p != NULL) 
	{
		treeprint(p->left);
		printf("%4d %s\n",p->count,p->word);
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
		*w++ = '\0';
	if(!isalpha(c)) {
		*w='\0';
		return c;
	}
	for (; --lim > 0;w++)
		if(!isalnum(*w = getch_KR())) {
			ungetch_KR(*w);
			break;
		}
	*w='\0';
	return word[0];

}
	
#define MAXWORD 100
struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

int main()
{
	struct tnode *root;
	char word[MAXWORD];

	root=NULL;
	while (getword(word,MAXWORD) != EOF)
	{
		printf("get ur word: %s\n",word);
		if (isalpha(word[0]))
			root = addtree(root,word);
	}
	treeprint(root);
	return 0;

}
