obj = binSearch.o insertSort.o mergeSort.o selectionSort.o test.o heapSort.o quickSort.o bwt.o tree.o hgc_stack.o new_align.o fasta.o ini.o
CFLAGS = -g -o2
test: $(obj)
	cc -o test $(obj)
test.o: algrithms.h hgc_stack.h new_align.h fasta.h
binSearch.o: binSearch.c
insertSort.o: insertSort.c
mergeSort.o: mergeSort.c
selectionSort.o: selectionSort.c
heapSort.o: heapSort.c
quickSort.o: quickSort.c
bwt.o: bwt.c
tree.o: tree.c
hgc_stack.o: hgc_stack.c
fasta.o: fasta.c
new_align.o: new_align.c ini.h
ini.o: ini.c

.PHONY:clean
clean:
	-rm test $(obj)
