CC=gcc# -D_POSIX_C_SOURCE=200112
CFLAGS=${CFLAGS_EXTRA} -I. -DGLEW_STATIC -std=c11 -pedantic
LDLIBS=-lrt -lGL -lglfw -lGLEW -lm -lSOIL -lfreetype
SRCS=util.c shader.c renderer.c rtiso.c map.c texture.c coord.c unit.c phys.c animation.c interaction.c data.c gl.c render_object.c text.c tilemap.c
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=renderer.h coord.h util.h shader.h linmath.h map.h texture.h coord.h unit.h phys.h animation.h interaction.h data.h gl.h render_object.h text.h tilemap.h
BINDIR?=bin

all: $(BINDIR)/rtiso

# TODO: separate debug build
$(BINDIR)/rtiso: $(OBJS)
	@mkdir -p $(@D)
	$(CC) -g -o $(BINDIR)/rtiso $(OBJS) $(CFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -g $(CFLAGS) $(LDLIBS) -c $^ -o $@
