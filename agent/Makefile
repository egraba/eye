########################
# Makefile for OpenBSD #
########################

# Compiler rules
CC	= gcc -g
CFLAGS	= -W -Wall -ansi -pedantic
LINKS	= 

# Project settings
EXEC	= eye
SRC	= eye.c info.c usage.c
OBJS	= ${SRC:.c=.o}

# Directives

all: ${OBJS}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJS} ${LINKS}

clean:
	rm -f $(EXEC)
	rm -f *.o
	rm -f *~

