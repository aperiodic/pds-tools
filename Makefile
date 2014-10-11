LIBDIR := ./lib
HASHDIR := $(LIBDIR)/hash
CSRCDIR := ./src

INCLUDE := -I$(CSRCDIR) \
					 -I$(HASHDIR)

CSRC := $(CSRCDIR)/*.c \
				$(HASHDIR)/hashtable.c \
				$(HASHDIR)/hashtable_itr.c

CCFLAGS := --std=c99 -lm
DEMO_FLAGS= $(CCFLAGS) -Winline -L./bin -lpdstools
LIB_FLAGS := $(CCFLAGS) -lc -shared -fPIC

BUILDDIR := ./bin
DEMO_TARGET := $(BUILDDIR)/pds-parse-demo
LIB_TARGET := $(BUILDDIR)/libpdstools.so

$(LIB_TARGET): $(BUILDDIR) $(CSRCDIR)/*.c
	gcc $(LIB_FLAGS) -o $(LIB_TARGET) $(INCLUDE) $(CSRC)

demo: $(LIB_TARGET) $(CSRCDIR)/*.c $(LIB_TARGET)
	gcc $(DEMO_FLAGS) -o $(DEMO_TARGET) $(INCLUDE) $(CSRC) $(CSRCDIR)/demo/main.c

debug: $(LIB_TARGET) $(CSRCDIR)/*.c
	gcc -DDEBUG=true $(CCFLAGS) -o $(TARGET) $(INCLUDE) $(CSRC)

run: demo
	LD_LIBRARY_PATH=./bin $(DEMO_TARGET) samples/hirise.lbl

.PHONY: $(BUILDDIR)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

.PHONY: force
force:
	make clean main
