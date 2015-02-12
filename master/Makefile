########################
# Makefile for OpenBSD #
########################

# Compiler rules
CC	= gcc -g
CFLAGS	= -Wall -Wextra -ansi -pedantic
LINKS	= 

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
	rm -f *~

