CC=gcc
CFLAGS=-I/usr/include/freetype2 -I. -DGLEW_STATIC -std=gnu99 -pedantic
LDLIBS=-lrt -lGL -lglfw -lGLEW -lm -lSOIL -lfreetype
SRCS=util.c shader.c renderer.c rtiso.c map.c texture.c coord.c unit.c
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=renderer.h coord.h util.h shader.h sprite.h linmath.h map.h texture.h coord.h unit.h
BINDIR?=bin

all: $(BINDIR)/rtiso

$(BINDIR)/rtiso: $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(BINDIR)/rtiso $(OBJS) 

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDLIBS) -c $^ -o $@
