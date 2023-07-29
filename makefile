CC = gcc
CFLAGS = -std=c11 -g -static -Wall -Werror

kcc: kcc.c
	$(CC) $(CFLAGS) -o kcc kcc.c

test: kcc
	./test.sh

clean:
	rm -f kcc *.o *~ tmp*

.PHONY: test clean

