CC = gcc
CFLAGS = -pedantic -Wall -Werror -Wextra -std=c99 -fPIC -g -fvisibility=hidden
LDFLAGS = -shared

LIB = libmalloc.so

VPATH = src/
OBJS = realloc.o calloc.o free.o malloc.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIB) $(OBJS)

clean:
	$(RM) $(OBJS) $(LIB)

test:
	bash tests/testsuite.sh
