INCLUDE := -I/usr/lib/jvm/java-7-openjdk/include \
					 -I/usr/lib/jvm/java-7-openjdk/include/linux \
					 -I../../lib/hash

CSRCDIR := ../../src
CSRC := pdstools.c \
				$(CSRCDIR)/buff.c \
				$(CSRCDIR)/parse.c \
				$(CSRCDIR)/pool.c \
				$(CSRCDIR)/token.c \
				$(CSRCDIR)/tokenize.c \
				$(CSRCDIR)/token_stream.c \
				$(CSRCDIR)/util.c

main: pdstools.c PDSTools.h
	gcc -o libpdstools.so -std=c99 -lc -shared -fPIC \
		$(INCLUDE) \
		$(CSRC)

PDSTools.class: PDSTools.java
	javac PDSTools.java

PDSTools.h: PDSTools.class
	javah PDSTools

.PHONY: clean
clean:
	rm -f *.so PDSTools.h *.class

.PHONY: run
run:
	java -Djava.library.path=. PDSTools
