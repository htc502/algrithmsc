/* quicksort */
int partition(int *array, int start, int stop)
{
  int x,i,j;
  x=array[stop];
  i=start-1;
  for(j=start;j<stop;j++)
    {
      if(array[j]<=x)
	{
	  i += 1;
	  int temp=array[i];
	  array[i]=array[j];
	  array[j]=temp;
	}
    }
  int p=array[i+1];
  array[i+1]=array[stop];
  array[stop]=p;
  return(i+1);
}

void quickSort(int *array,int start, int stop)
{
  int partition(int *,int,int);
  int q=partition(array,start,stop);
  if(q-1 >= start)
    quickSort(array,start,q-1);
  if(stop >= q+1)
    quickSort(array,q+1,stop);
}

