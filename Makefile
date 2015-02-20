.PHONY: all clean
CC=gcc
CFLAGS=-I. -lreadline -lm -std=c99 -Wall -ggdb

SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))

all: ex_bonus1

clean:
	rm *.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ex_bonus1: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
