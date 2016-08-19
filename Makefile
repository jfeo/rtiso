SRCDIR=$(shell pwd)/src
BINDIR=$(shell pwd)/bin
OBJDIR=$(shell pwd)/obj
export BINDIR
export OBJDIR

.PHONY: build
build:
	$(MAKE) -C $(SRCDIR) build

.PHONY: run
run: build
	$(BINDIR)/ctw

.PHONY: clean
clean:
	$(MAKE) -C $(SRCDIR) clean
