CFLAGS=-Wall --std=c11

all: test

clean:
	rm -rf \
	  scel.o scel_test scel_test.o \
	  heap.o heap_test heap_test.o \
	  stack_test stack_test.o
.PHONY: clean

test: heap_test scel_test stack_test
	./scel_test
	./heap_test
	./stack_test
.PHONY: test

heap_test: heap_test.o heap.o scel.o
	$(CC) -o heap_test heap_test.o heap.o scel.o

scel_test: scel_test.o scel.o
	$(CC) -o scel_test scel_test.o scel.o

stack_test: stack_test.o
	$(CC) -o stack_test stack_test.o

heap.o: heap.c heap.h scel.h result.h
heap_test.o: heap.h testing.h
scel.o: scel.c scel.h utf8.h
scel_test.o: scel.h testing.h
stack_test.o: stack.h panic.h testing.h
