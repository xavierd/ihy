CAMLC=ocamlopt
CAMLFLAGS=-output-obj bigarray.cmxa
OCAMLPATH=${shell ocamlc -where}

CC=gcc
CFLAGS=-Wall -W -pedantic -Wlong-long -I${OCAMLPATH}
LDFLAGS=-L${OCAMLPATH} ${NATIVECCLIBS} -lasmrun -lbigarray

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

CSRC=wav.c ihy.c interface.c huffman.c main.c
CINTERFACE=wav.h ihy.h interface.h huffman.h
COBJ=${CSRC:.c=.o}
CAMLSRC=ondelettes.ml
CAMLINTERFACE=ondelettes.mli
CAMLOBJ=${CAMLSRC:.ml=_ml.o}
CAMLCMI=${CAMLINTERFACE:.mli=.cmi}
CAMLO=${CAMLSRC:.ml=.o}
CAMLCMX=${CAMLSRC:.ml=.cmx}

EXEC=ihyconvert

all: ${EXEC}

${EXEC}: ${COBJ} ${CAMLCMI} ${CAMLOBJ} ${CINTERFACE}
	${CC} -o ${OUTPUT}${EXEC} ${CAMLOBJ} ${COBJ} ${LDFLAGS}

%.o: %.c
	${CC} ${CFLAGS} -c $< -I${OCAMLPATH}

%_ml.o: %.ml
	${CAMLC} ${CAMLFLAGS} $< -o $@

%.cmi: %.mli
	${CAMLC} $<

clean:
	rm -rf ${COBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI} ${CAMLO}
	rm -rf ${OUTPUT}${EXEC}

-include ${OCAMLPATH}/Makefile.config
