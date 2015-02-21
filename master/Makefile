########################
# Makefile for OpenBSD #
########################

# Compiler rules
IDIR	= ../include
CC	= gcc -g
CFLAGS	= -Wall -Wextra -ansi -pedantic -I${IDIR}

# Project settings
EXEC	= master
SRC	= master.c parser.c
OBJS	= ${SRC:.c=.o}

# Directives
all: ${OBJS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJS}

clean:
	rm -f ${EXEC}
	rm -f *.o
	rm -f *~

