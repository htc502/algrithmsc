#include<stdio.h>
#include<string.h>
/* bs query query against against,len is the length of against, be sure that against is sorted in a/descending order */
int bs(char* query, char** against,int len) //bs for binary search
{
  int cond;
  int low,high,mid;

  low = 0;
  high = len-1;
  while (low <= high) {
    mid = (low + high) / 2;
    if ((cond = strcmp(query, *(against + mid) )) < 0)
      high = mid - 1;
    else if (cond > 0)
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}
/* for binary search , there is a loop process involving narrowing the upper and lower bound of the against array, the key point is to chaining the the current boundary with that in the next loo */
