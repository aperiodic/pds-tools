LIBDIR := ./lib
HASHDIR := $(LIBDIR)/hash
CSRCDIR := ./src

INCLUDE := -I$(CSRCDIR) \
					 -I$(HASHDIR)

CSRC := $(CSRCDIR)/*.c \
				$(HASHDIR)/hashtable.c \
				$(HASHDIR)/hashtable_itr.c

CCFLAGS := --std=c99 -lm
DEMO_FLAGS= $(CCFLAGS) -Winline
LIB_FLAGS := $(CCFLAGS) -lc -shared -fPIC

BUILDDIR := ./bin
DEMO_TARGET := $(BUILDDIR)/pds-parse-demo
LIB_TARGET := $(BUILDDIR)/libpdstools.so

main: $(BUILDDIR) $(CSRCDIR)/*.c
	gcc $(LIB_FLAGS) -o $(LIB_TARGET) $(INCLUDE) $(CSRC)

demo: $(BUILDDIR) $(CSRCDIR)/*.c
	gcc $(DEMO_FLAGS) -o $(DEMO_TARGET) $(INCLUDE) $(CSRC)

debug: $(BUILDDIR) $(CSRCDIR)/*.c
	gcc -DDEBUG=true $(CCFLAGS) -o $(TARGET) $(INCLUDE) $(CSRC)

.PHONY: $(BUILDDIR)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

.PHONY: force
force:
	make clean main
