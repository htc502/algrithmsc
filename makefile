obj = binSearch.o insertSort.o mergeSort.o selectionSort.o test.o heapSort.o quickSort.o bwt.o tree.o
test: $(obj)
	cc -o test $(obj)
test.o: algrithms.h 
binSearch.o: binSearch.c
insertSort.o: insertSort.c
mergeSort.o: mergeSort.c
selectionSort.o: selectionSort.c
heapSort.o: heapSort.c
quickSort.o: quickSort.c
bwt.o: bwt.c
tree.o: tree.c

.PHONY:clean
clean:
	-rm test $(obj)
