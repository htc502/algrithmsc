#include <limits.h>
#include <stdio.h>
//this is a iterative fxn which is a liitle bit hard for understanding, it has strange parameter list as is needed by the nesting iterative call by itself

void merge(int* array,int len, int left,int mid, int right)
{
  //left,mid,right are all start from zero
  void printarray(int*,int,int,int);
  int L[mid-left+1+1], R[right-mid+1];//one more cell for the sentinel value
  //when dealing with sorted sequence combine problem, sentinel value is a simple and good solution

  //copy the left-right part elements to L and R; insert dummy sentinel
  int i=0;
  for(i=left;i<=right;i++)
    i>mid ? (R[i-mid-1]=array[i]):(L[i-left]=array[i]);
  L[mid-left+1]=INT_MAX;
  R[right-mid]=INT_MAX;

  //merge L and R
  int li=0,ri=0,x=left; //li=left index;ri=right index;x-array index
  while((L[li]!= INT_MAX) ||(R[ri]!=INT_MAX)) {
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
  if(end > start){
    int split=(end+start)/2;
    mergeSort(array,start,split);
    mergeSort(array,split+1,end);
    merge(array,(end-start+1),start,split,end);
  }
}

