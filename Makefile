.PHONY: clean
.PHONY: force
.PHONY: build_dir

LIBDIR := ./lib
HASHDIR := $(LIBDIR)/hash
CSRCDIR := ./src

INCLUDE := -I$(CSRCDIR) \
					 -I$(HASHDIR)

CSRC := $(CSRCDIR)/*.c \
				$(HASHDIR)/hashtable.c \
				$(HASHDIR)/hashtable_itr.c

CCFLAGS := --std=c99 -Winline -lm $(INCLUDE)

BUILDDIR := ./build
TARGET := $(BUILDDIR)/pds-parse

main: $(BUILDDIR) $(CSRCDIR)/*.c
	gcc $(CCFLAGS) -o $(TARGET) $(CSRC)

debug: $(BUILDDIR) $(CSRCDIR)/*.c
	gcc -DDEBUG=true $(CCFLAGS) -o $(TARGET) $(CSRC)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

force:
	make clean main
