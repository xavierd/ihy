CAMLC=ocamlopt
CAMLFLAGS=-output-obj bigarray.cmxa
OCAMLPATH=${shell ocamlc -where}

CC=gcc
CFLAGS=-Wall -W -pedantic -I${OCAMLPATH}
LDFLAGS=

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

CSRC=pcm.c ihy.c main.c
COBJ=${CSRC:.c=.o}
CAMLSRC=
CAMLOBJ=${CAMLSRC:.ml=.o}

CAMLCMI=${CAMLSRC:.ml=.cmx}
CAMLCMX=${CAMLSRC:.ml=.cmi}

EXEC=ihydeflate

all: ${EXEC}

${EXEC}: ${COBJ} ${CAMLOBJ}
	${CC} -o ${OUTPUT}${EXEC} ${COBJ} ${CAMLOBJ} ${LDFLAGS} -L${OCAMLPATH} ${NATIVECCLIBS} -lasmrun -lbigarray

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

%.o: %.ml
	${CAMLC} ${CAMLFLAGS} $< -o $@

clean:
	rm -rf ${COBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI}
	rm -rf ${OUTPUT}${EXEC}

-include ${OCAMLPATH}/Makefile.config
