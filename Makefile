main: parse.c buff.c token.c token_stream.c pool.c main.c
	gcc -I. --std=c99 -o pds-parse main.c buff.c token.c token_stream.c pool.c parse.c

.PHONY: clean
.PHONY: force

clean:
	rm pds-parse

force:
	make clean main
