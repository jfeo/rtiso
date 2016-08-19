CC=gcc
CFLAGS=-I. -DGLEW_STATIC
LDLIBS=-lGL -lglfw -lGLEW
SRCS=ctw.c renderer.c
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=renderer.h
BINDIR?=bin

.PHONY: build
build: $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/ctw $(OBJS) $(CFLAGS) $(LDLIBS)

%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(LIBS) -c $^ -o $(OBJDIR)/$@

.PHONY: clean
clean:
	rm -f *.o ctw
