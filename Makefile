CFLAGS=-Wall --std=c11 -g

all: test

clean:
	rm -rf \
	  panic.o \
	  scel.o scel_test scel_test.o \
	  heap.o heap_test heap_test.o \
	  stack_test stack_test.o \
	  symbol.o symbol_test symbol_test.o \
	  utf8_test utf8_test.o utf8.o
.PHONY: clean

test: heap_test scel_test stack_test symbol_test utf8_test
	./scel_test
	./heap_test
	./stack_test
	./utf8_test
	./symbol_test
.PHONY: test

heap_test: heap_test.o heap.o scel.o
	$(CC) -o heap_test heap_test.o heap.o scel.o

scel_test: scel_test.o scel.o
	$(CC) -o scel_test scel_test.o scel.o

stack_test: stack_test.o
	$(CC) -o stack_test stack_test.o

symbol_test: symbol_test.o symbol.o scel.o
	$(CC) -o symbol_test symbol_test.o symbol.o scel.o

utf8_test: utf8_test.o utf8.o
	$(CC) -o utf8_test utf8_test.o utf8.o

heap.o: heap.c heap.h scel.h result.h
heap_test.o: heap.h testing.h
scel.o: scel.c scel.h utf8.h
scel_test.o: scel.h testing.h
stack_test.o: stack.h panic.h testing.h
symbol.o: symbol.c panic.h symbol.h scel.h utf8.h
symbol_test.o: symbol.h panic.h testing.h
utf8.o: utf8.c utf8.h
utf8_test.o: utf8.h testing.h
