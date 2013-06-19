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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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
