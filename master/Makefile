########################
# Makefile for FreeBSD #
########################

# Compiler rules
CC	= clang
CFLAGS	= -Wall -pedantic ${INCLUDES}
INCLUDES= -I/usr/local/include
LINKS	= -L/usr/local/lib -lyajl

# Project settings
EXEC	= master
SRC	= master.c jsonParser.c
OBJS	= ${SRC:.c=.o}

# Directives

all: ${OBJS}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJS} ${LINKS}

clean:
	rm -f $(EXEC)
	rm -f *.o

analyze:
	$(CC) --analyze ${SRC}


