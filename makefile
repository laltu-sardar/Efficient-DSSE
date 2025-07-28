all: cleanHelper clean main trie source sha256 compile run
cleanHelper:
	gcc -g  -c test.c -o test.o
sha256:
	gcc -g  -c sha256.c -o sha256.o
trie:
	gcc -g -Wall -c trie.c -o trie.o
source:
	gcc -g -Wall -c source.c -o source.o
main:
	gcc -g -Wall -c main.c -o main.o
compile: cleanHelper clean sha256 trie source main
	gcc -Wall sha256.o trie.o source.o main.o -o dsse.o
run:
	time ./dsse.o
gdb: compile
	gdb ./dsse.o
clean:
	rm *.o
test.o:
	gcc -g -Wall -c test.c -o test.o
test: sha256 test.o source compile
	gcc -Wall sha256.o source.o test.o -o test.out
	./test.out
