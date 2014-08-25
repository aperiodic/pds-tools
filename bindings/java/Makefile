INCLUDE := -I../../src \
           -I../../lib/hash \
           -I/usr/lib/jvm/java-7-openjdk/include \
           -I/usr/lib/jvm/java-7-openjdk/include/linux

CSRCDIR := ../../src
CLIBDIR := ../../lib
HASHDIR := $(CLIBDIR)/hash
CSRC := pdstools.c \
				$(CSRCDIR)/buff.c \
				$(CSRCDIR)/parse.c \
				$(CSRCDIR)/pool.c \
				$(CSRCDIR)/token.c \
				$(CSRCDIR)/tokenize.c \
				$(CSRCDIR)/token_stream.c \
				$(CSRCDIR)/util.c \
				$(HASHDIR)/hashtable.c \
				$(HASHDIR)/hashtable_itr.c

JPACKAGE := org.aperiodic
JPACKAGEDIR := org/aperiodic
JCLASS := PDSTools

main: pdstools.c $(JCLASS).h $(JCLASS).jar
	gcc -o libpdstools.so -std=c99 -lc -shared -fPIC \
		$(INCLUDE) \
		$(CSRC)

$(JPACKAGEDIR)/$(JCLASS).class: $(JPACKAGEDIR)/*.java
	javac $(JPACKAGEDIR)/*.java

$(JCLASS).jar: $(JPACKAGEDIR)/$(JCLASS).class
	jar cmf Manifest.txt $(JCLASS).jar $(JPACKAGEDIR)/*

$(JCLASS).h: $(JPACKAGEDIR)/$(JCLASS).class
	javah $(JPACKAGE).$(JCLASS)

.PHONY: clean
clean:
	rm -f *.so $(JCLASS).h $(JPACKAGEDIR)/*.class

.PHONY: run
run:
	java -Djava.library.path=. $(JPACKAGE).$(JCLASS)
