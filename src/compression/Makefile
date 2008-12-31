CC=gcc
CFLAGS=-Wall -W -pedantic

DEBUG=yes
ifeq (${DEBUG},yes)
	CFLAGS+=-g -D DEBUG
	OUTPUT=
else
	CFLAGS+=-O2
	OUTPUT=../../bin/
endif

LDFLAGS=

SRC=pcm.c ihy.c main.c
OBJ=${SRC:.c=.o}

EXEC=ihydeflate

all: ${EXEC}

${EXEC}: ${OBJ}
	${CC} -o ${OUTPUT}${EXEC} ${OBJ} ${LDFLAGS}

%.o: %.c
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	rm -rf ${OBJ}
	rm -rf ${OUTPUT}${EXEC}
