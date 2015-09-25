obj = binSearch.o insertSort.o mergeSort.o selectionSort.o test.o heapSort.o quickSort.o bwt.o tree.o stack.o new_align.o fasta.o
test: $(obj)
	cc -o test $(obj)
test.o: algrithms.h stack.h new_align.h fasta.h
binSearch.o: binSearch.c
insertSort.o: insertSort.c
mergeSort.o: mergeSort.c
selectionSort.o: selectionSort.c
heapSort.o: heapSort.c
quickSort.o: quickSort.c
bwt.o: bwt.c
tree.o: tree.c
stack.o: stack.c
fasta.o: fasta.c
new_align.o: new_align.c

.PHONY:clean
clean:
	-rm test $(obj)
