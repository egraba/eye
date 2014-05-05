########################
# Makefile for FreeBSD #
########################

# Compiler rules
CC	= clang
CFLAGS	= -Wall -pedantic
LINKS	= -lm

# Project settings
EXEC	= eye
SRC	= eye.c computerInfo.c computerUsage.c
OBJS	= ${SRC:.c=.o}

# Directives

all: ${OBJS}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJS} ${LINKS}

clean:
	rm -f $(EXEC)
	rm -f *.o

analyze:
	$(CC) --analyze ${SRC}


