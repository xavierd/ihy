CAMLC=ocamlopt
CAMLFLAGS=-output-obj bigarray.cmxa
OCAMLPATH=${shell ocamlc -where}

CC=gcc
CFLAGS=-Wall -W -pedantic -Wno-long-long -I${OCAMLPATH} -I./
LDFLAGS=-L${OCAMLPATH} -L/usr/local/lib ${NATIVECCLIBS} -lao -lasmrun -lbigarray -lpthread

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

CSRC=input/wav.c output/ao.c ihy.c wavelet.c lossless/huffman.c main.c
CINTERFACE=input/wav.h output/ao.h ihy.h wavelet.h lossless/huffman.h
COBJ=${CSRC:.c=.o}
CAMLSRC=wavelets/haar.ml
CAMLINTERFACE=wavelets/haar.mli
CAMLOBJ=${CAMLSRC:.ml=_ml.o}
CAMLCMI=${CAMLINTERFACE:.mli=.cmi}
CAMLO=${CAMLSRC:.ml=.o}
CAMLCMX=${CAMLSRC:.ml=.cmx}

EXEC=ihyconvert

all: ${EXEC}

${EXEC}: ${COBJ} ${CAMLCMI} ${CAMLOBJ} ${CINTERFACE}
	${CC} -o ${OUTPUT}${EXEC} ${CAMLOBJ} ${COBJ} ${LDFLAGS}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%_ml.o: %.ml
	${CAMLC} ${CAMLFLAGS} $< -o $@

%.cmi: %.mli
	${CAMLC} $< -o $@

clean:
	rm -rf ${COBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI} ${CAMLO}
	rm -rf ${OUTPUT}${EXEC}

-include ${OCAMLPATH}/Makefile.config
