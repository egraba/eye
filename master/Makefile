########################
# Makefile for FreeBSD #
########################

# Compiler rules
CC	= clang
CFLAGS	= -Wall -pedantic
LINKS	= -Lyajl

# Project settings
EXEC	= master
SRC	= master.c
OBJS	= ${SRC:.c=.o}

# Directives

all: ${OBJS}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJS} ${LINKS}

clean:
	rm -f $(EXEC)
	rm -f *.o

analyze:
	$(CC) --analyze ${SRC}


