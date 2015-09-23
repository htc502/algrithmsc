obj = new_align.o fasta.o
CFLAGS = -Wall -g -o0
nw: $(obj)
	cc  -o nw $(obj)
new_align.o : fasta.h
fasta.o: fasta.h

.PHONY:clean
clean:
	-rm nw $(obj)
