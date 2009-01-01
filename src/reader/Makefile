CC=gcc
CFLAGS=-Wall -W -pedantic

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

LDFLAGS=

SRC=main.c
OBJ=${SRC:.c=.o}

EXEC=ihyreader

all: ${EXEC}

${EXEC}: ${OBJ}
	${CC} -o ${OUTPUT}${EXEC} ${OBJ} ${LDFLAGS}

%.o: %.c
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	rm -rf ${OBJ}
	rm -rf ${OUTPUT}${EXEC}
