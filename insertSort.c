#include <stdio.h>

// insertion sort on the fly:
// in order to do insertion sort, we have to :
// 1. initialize an empty sorted_array
// 3. chose one element in the to_sort_array; check for its position in sorted_array and insert it into the position
// 4. do step three until there is no elelment in to_sort_array
// 5. we are done 
// but this is not an in place version, in order to do sortation in place, we have to modify the process.
//just use a separator to split the original array into two parts:
//the left part is sorted_array while the right part is to_sort_array and it comes like this:
void insertSort(int* array,int n)
{
  int sp=1,key=0; //denote the separator line between i and i+1th element as i+1(starts from 0); the first number dosn't need to be sorted, start from the second element of array...
  for(;sp<=n-1;sp++)
    {
      key=*(array+sp);
      int i=sp-1; //index for the sorted array:0~sp-1(dose not need to compare with itself)
      //find the right place
      while(i>=0 && *(array+i) > key)/* loop from sp-1 to 0 */
	{
	  *(array+i+1)=*(array+i);
	  i--;
	}
      //do the insertion
      *(array+i+1)=key;
    }
}

