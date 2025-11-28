CC = gcc# -D_POSIX_C_SOURCE=200112
CFLAGS=${CFLAGS_EXTRA} -I. -DGLEW_STATIC -std=c11 -pedantic
LDLIBS=-lrt -lGL -lglfw -lGLEW -lm -lSOIL -lfreetype
BINDIR?=bin

LIB_SRCS = animation.c coord.c data.c gl.c interaction.c map.c phys.c \
					 render_object.c renderer.c shader.c text.c texture.c tilemap.c \
					 timer.c unit.c util.c
LIB_OBJS = $(addprefix $(OBJDIR)/,$(LIB_SRCS:.c=.o))

# Main executable file definitions
RTISO_SRC = rtiso.c
RTISO_OBJ = $(OBJDIR)/rtiso.o

# Test file definitions
# test.c includes all other test files and creates a master test suite
# which means that all the test files are compiled into a single object
# which is then linked to the library objects to form the test executable.
TEST_SRCS = coord_test.c
TEST_ENTRY_SRC = test.c
TEST_OBJ = $(OBJDIR)/test.o

all: $(BINDIR)/rtiso

# TODO: separate debug build
$(BINDIR)/rtiso: $(RTISO_OBJ) $(LIB_OBJS)
	@mkdir -p $(@D)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDLIBS) 

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -g -o $@ $(CFLAGS) -c $^

# The single test object is linked with the library objects
$(BINDIR)/test: $(TEST_OBJ) $(LIB_OBJS)
	@mkdir -p $(@D)
	$(CC) -g -o $@ $(CFLAGS) $^ -lcmocka $(LDLIBS)

# Because we include test source files directly in the test entrypoint, we only
# need to build the test entrypoint object
$(OBJ_TEST): $(TEST_ENTRY_SRC) $(TEST_SRCS)
	@mkdir -p $(@D)
	$(CC) -g -o $@ $(CFLAGS) -c $(TEST_ENTRY_SRC)
