CC=gcc
CFLAGS=-I.
LIBS=-lGL
DEPS=

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ctw: ctw.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: ctw

.PHONY: clean

clean:
	rm -f *.o ctw
