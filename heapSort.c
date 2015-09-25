/** heap sort **/

/* auxillary funcs */
int parent(int i)
{
  return i%2==0 ? i/2-1 : i/2;
}

int left(int i)
{
  return 2*i+1;/* index start from 0 */
}

int right(int i)
{
  return 2*i+2;
}

void max_heapify(int *array,int narray,int iroot)
{
  /* keep the heap with the property a[parent(i)]>=a[i] */
  int parent(int);
  int left(int);
  int right(int);

  int l,r,largest;
  l=left(iroot);
  r=right(iroot);
  if(l<narray && array[l]> array[iroot])
    largest=l;
  else
    largest=iroot;
  if(r<narray && array[r]>array[largest])
    largest=r;
  if(largest != iroot){
    int m=array[iroot];
    array[iroot]=array[largest];
    array[largest]=m;
    max_heapify(array,narray,largest);
  }
}

/* build a max heap from buttom */
void  build_max_heap(int *array,int narray)
{
  void max_heapify(int *,int,int);
  int m=narray/2;
  for(;m>=0;m--)
    {
      max_heapify(array,narray,m);
    }
}
/* heap sort */
void heapSort(int *array,int narray)
{
  void build_max_heap(int*,int);
  void max_heapify(int*,int,int);
  build_max_heap(array,narray);
  int x=0,heapsize=narray;
  for(x=narray-1;x>=1;x--)
    {
      int tmp=array[0];
      array[0]=array[x];
      array[x]=tmp;
      max_heapify(array,--heapsize,0);
    }
}
