CAMLC=ocamlopt
CAMLFLAGS=-output-obj bigarray.cmxa
OCAMLPATH=${shell ocamlc -where}

CC=gcc
CFLAGS=-Wall -W -pedantic -I${OCAMLPATH}
LDFLAGS=${NATIVECCLIBS} -lasmrun -lbigarray

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

CSRC=wav.c ihy.c main.c
COBJ=${CSRC:.c=.o}
CAMLSRC=ondelettes.ml
CAMLOBJ=${CAMLSRC:.ml=_ml.o}

CAMLCMI=${CAMLSRC:.ml=.cmx}
CAMLCMX=${CAMLSRC:.ml=.cmi}
CAMLO=${CAMLSRC:.ml=.o}

EXEC=ihyconvert

all: ${EXEC}

${EXEC}: ${COBJ} ${CAMLOBJ}
	${CC} -o ${OUTPUT}${EXEC} ${CAMLOBJ} ${COBJ} \
	    -L${OCAMLPATH} ${LDFLAGS}

%.o: %.c
	${CC} ${CFLAGS} -c $< -I${OCAMLPATH}

%_ml.o: %.ml
	${CAMLC} ${CAMLFLAGS} $< -o $@

clean:
	rm -rf ${COBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI} ${CAMLO}
	rm -rf ${OUTPUT}${EXEC}

-include ${OCAMLPATH}/Makefile.config
