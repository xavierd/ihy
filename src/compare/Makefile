CC=gcc
CFLAGS=-O2 -Wall -W -pedantic
LDFLAGS=-L/usr/local/lib -lfftw3 -lm

SRC=codecs/wav.c fftux.c
OBJ=${SRC:.c=.o}

EXEC=fftux

all: ${EXEC}

${EXEC}: ${OBJ}
	${CC} -o ${EXEC} ${OBJ} ${LDFLAGS}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${OBJ}
	rm -rf ${EXEC}
