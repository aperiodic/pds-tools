main: parse.c buff.c token.c token_stream.c tokenize.c pool.c util.c main.c
	gcc -I. -I./hash --std=c99 -Winline -lm -o pds-parse main.c buff.c token.c tokenize.c token_stream.c pool.c parse.c util.c hash/hashtable.c hash/hashtable_itr.c

debug: parse.c buff.c token.c token_stream.c pool.c util.c main.c
	gcc -DDEBUG=true -I. -I./hash --std=c99 -Winline -lm -o pds-parse main.c buff.c token.c tokenize.c token_stream.c pool.c parse.c util.c hash/hashtable.c hash/hashtable_itr.c

.PHONY: clean
.PHONY: force

clean:
	rm pds-parse

force:
	make clean main
