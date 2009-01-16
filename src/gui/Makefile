CC=gcc
CFLAGS=-Wall -W -pedantic `pkg-config --cflags gtk+-2.0`

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

LDFLAGS=`pkg-config --libs gtk+-2.0`

SRC=main.c
OBJ=${SRC:.c=.o}

EXEC=ihyplayer

all: ${EXEC}

${EXEC}: ${OBJ}
	${CC} -o ${OUTPUT}${EXEC} ${OBJ} ${LDFLAGS}

%.o: %.c
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	rm -rf ${OBJ}
	rm -rf ${OUTPUT}${EXEC}
