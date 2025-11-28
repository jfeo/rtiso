SRCDIR=$(shell pwd)/src
BINDIR=$(shell pwd)/bin
OBJDIR=$(shell pwd)/obj
export BINDIR
export OBJDIR

.PHONY: build
build:
	$(MAKE) -C $(SRCDIR)

.PHONY: run
run: build
	$(BINDIR)/rtiso

.PHONY: test
test:
	$(MAKE) -C $(SRCDIR) $(BINDIR)/test
	$(BINDIR)/test

.PHONY: clean
clean:
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
