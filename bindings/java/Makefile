INCLUDE := -I/usr/lib/jvm/java-7-openjdk/include \
					 -I/usr/lib/jvm/java-7-openjdk/include/linux \
					 -I../../lib/hash

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

main: pdstools.c PDSTools.h
	gcc -o libpdstools.so -std=c99 -lc -shared -fPIC \
		$(INCLUDE) \
		$(CSRC)

$(JPACKAGEDIR)/*.class: $(JPACKAGEDIR)/*.java
	javac $(JPACKAGEDIR)/*.java

PDSTools.h: $(JPACKAGEDIR)/PDSTools.class
	javah $(JPACKAGE).PDSTools

.PHONY: clean
clean:
	rm -f *.so PDSTools.h $(JPACKAGEDIR)/*.class

.PHONY: run
run:
	java -Djava.library.path=. org.aperiodic.PDSTools
