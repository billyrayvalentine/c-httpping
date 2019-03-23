CC=gcc
CFLAGS=-m64 -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
#CFLAGS=-m64 -std=c99  -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lcurl -lpopt
PREFIX=/usr/local

main:
	install -d bin
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDLIBS) httpping.c -o bin/httpping

install:
	install -m0755 bin/httpping $(PREFIX)/bin
