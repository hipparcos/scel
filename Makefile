CFLAGS=-Wall --std=c11

all: repl

clean:
	rm -rf scel.o scel-test
.PHONY: clean

test: scel_test
	./scel_test
.PHONY: test

scel_test: scel_test.o scel.o
	$(CC) -o scel_test scel_test.o scel.o

scel_test.p: scel.h
scel.o: scel.c scel.h utf8.h
