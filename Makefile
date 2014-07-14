main: parse.c buff.c token.c token_stream.c pool.c main.c
	gcc -I. -I./hash --std=c99 -lm -o pds-parse main.c buff.c token.c token_stream.c pool.c parse.c hash/hashtable.c

.PHONY: clean
.PHONY: force

clean:
	rm pds-parse

force:
	make clean main
