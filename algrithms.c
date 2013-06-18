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
