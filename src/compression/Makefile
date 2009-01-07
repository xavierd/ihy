CAMLC=ocamlopt
CAMLFLAGS=bigarray.cmxa -output-obj
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

CSRC=pcm.c ihy.c main.c
COBJ=${CSRC:.c=.o}
CAMLSRC=ondelettes.ml
CAMLOBJ=${CAMLSRC:.ml=.o}

CAMLCMI=${CAMLSRC:.ml=.cmx}
CAMLCMX=${CAMLSRC:.ml=.cmi}

EXEC=ihydeflate

all: ${EXEC}

${EXEC}: ${COBJ} ${CAMLOBJ}
	${CC} -o ${OUTPUT}${EXEC} ${CAMLOBJ} ${COBJ} \
	    -L${OCAMLPATH} ${LDFLAGS}

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $< -I${OCAMLPATH}

%.o: %.ml
	${CAMLC} ${CAMLFLAGS} $< -o $@

clean:
	rm -rf ${COBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI}
	rm -rf ${OUTPUT}${EXEC}

-include ${OCAMLPATH}/Makefile.config
