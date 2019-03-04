CC=gcc
CFLAGS=-m64 -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
LDFLAGS=-lcurl
PREFIX=/usr/local

main:
	install -d bin
	$(CC) $(CFLAGS) $(LDFLAGS) httpping.c -o bin/httpping

install:
	install -m0755 bin/httpping $(PREFIX)/bin
