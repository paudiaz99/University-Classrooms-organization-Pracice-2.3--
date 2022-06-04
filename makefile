all: final_output

final_output: main.o linkedlist.o
	gcc mian.o linkedlist.o -o final_output -ggdb

main.o: main.c linkedlist.h
	gcc -c main.c -ggdb

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c -ggdb

.PHONY: clean
clean:
	rm *.o
	rm final_output

.PHONY: test
test: final_output
	@echo "First test"
	./final_output 1 2 3 4 5 6 7 8 9 10
	@echo "second test"
	./final_output 10 53 24 1 15 89 63 49 78
