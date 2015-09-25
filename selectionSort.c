#include <stdio.h>

void selectionSort(int* array,int n)
{
  //loop invariant: everytime array[0:i] is sorted in desreasing order
  int i=0,temp=0,j=0,min_ind=0;
  for(i=0;i<n;i++)
    {
      //find index of the max element in i:n-1
      min_ind=i;
      for(j=i;j<n;j++)
	{
	  if(array[j]<array[min_ind]) min_ind=j;
	}
      //switch
      temp=array[min_ind];
      array[min_ind]=array[i];
      array[i]=temp;
    }
}

//i want to know is this bubble sort?
