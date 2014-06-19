parse: parse.c buff.c token.c token_stream.c pool.c
	gcc -I. --std=c99 -o parse parse.c buff.c token.c token_stream.c pool.c

.PHONY: clean
.PHONY: force

clean:
	rm parse

force:
	make clean parse
