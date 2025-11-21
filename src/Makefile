CC=gcc# -D_POSIX_C_SOURCE=200112
CFLAGS=${CFLAGS_EXTRA} -I. -DGLEW_STATIC -std=c11 -pedantic
LDLIBS=-lrt -lGL -lglfw -lGLEW -lm -lSOIL -lfreetype
SRCS=util.c shader.c renderer.c rtiso.c map.c texture.c coord.c unit.c phys.c animation.c interaction.c
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=renderer.h coord.h util.h shader.h linmath.h map.h texture.h coord.h unit.h phys.h animation.h interaction.h
BINDIR?=bin

all: $(BINDIR)/rtiso

$(BINDIR)/rtiso: $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $(BINDIR)/rtiso $(OBJS) $(CFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDLIBS) -c $^ -o $@
