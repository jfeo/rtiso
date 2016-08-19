CC=gcc
CFLAGS=-I. -DGLEW_STATIC
LDLIBS=-lGL -lglfw -lGLEW
OBJS=ctw.o renderer.o
SRCS=ctw.c renderer.c
DEPS=renderer.h

run: ctw
	./ctw

ctw: $(OBJS)
	$(CC) -o ctw $(OBJS) $(CFLAGS) $(LIBS)

%.o: $(SRCS)
	$(CC) $(CFLAGS) $(LIBS) -c $^

.PHONY: clean

clean:
	rm -f *.o ctw
