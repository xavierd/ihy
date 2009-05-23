CAMLC=ocamlopt
CAMLFLAGS=-output-obj bigarray.cmxa
OCAMLPATH=${shell ocamlc -where}

CC=gcc
CFLAGS=-Wall -W -pedantic -Wno-long-long `pkg-config --cflags gtk+-2.0` -I${OCAMLPATH} -I./
LDFLAGS=-L${OCAMLPATH} -L/usr/local/lib ${NATIVECCLIBS} -lao -lasmrun -lbigarray -lpthread `pkg-config --libs gtk+-2.0`

ifeq (${DEBUG}, false)
	CFLAGS+=-O2
	OUTPUT=../../bin/
else
	CFLAGS+=-g -D DEBUG
	OUTPUT=
endif

CSRC=codecs/wav.c codecs/ihy.c audio_output/ao.c audio_output/wav_streaming.c \
     audio_output/ihy_streaming.c compression/quantization.c \
     compression/wavelet.c compression/ihy.c compression/huffman.c \
     utils/queue.c utils/buffer.c utils/half.c main_threading.c
COBJ=${CSRC:.c=.o}
CAMLSRC=compression/wavelets/haar.ml
CAMLINTERFACE=compression/wavelets/haar.mli
CAMLOBJ=${CAMLSRC:.ml=_ml.o}
CAMLCMI=${CAMLINTERFACE:.mli=.cmi}
CAMLO=${CAMLSRC:.ml=.o}
CAMLCMX=${CAMLSRC:.ml=.cmx}

CLISRC=cli.c
CLIOBJ=${CLISRC:.c=.o}
CLIEXEC=ihycli
GUISRC=gui.c
GUIOBJ=${GUISRC:.c=.o}
GUIEXEC=ihygui

all: ${CLIEXEC} ${GUIEXEC}

${CLIEXEC}: ${COBJ} ${CLIOBJ} ${CAMLCMI} ${CAMLOBJ}
	@${CC} -o ${OUTPUT}${CLIEXEC} ${CAMLOBJ} ${COBJ} ${CLIOBJ} ${LDFLAGS}
	@echo "  LD	${CLIEXEC}"

${GUIEXEC}: ${COBJ} ${GUIOBJ} ${CAMLCMI} ${CAMLOBJ}
	@${CC} -o ${OUTPUT}${GUIEXEC} ${CAMLOBJ} ${COBJ} ${GUIOBJ} ${LDFLAGS}
	@echo "  LD	${GUIEXEC}"

%.o: %.c %.h
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "  CC	$<"

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "  CC	$<"

%_ml.o: %.ml
	@${CAMLC} ${CAMLFLAGS} -I ./compression/wavelets $< -o $@
	@echo "  ML	$<"

%.cmi: %.mli
	@${CAMLC} $< -o $@
	@echo "  MLI	$<"

clean:
	rm -rf ${COBJ} ${CLIOBJ} ${GUIOBJ}
	rm -rf ${CAMLOBJ} ${CAMLCMX} ${CAMLCMI} ${CAMLO}
	rm -rf ${OUTPUT}${CLIEXEC} ${OUTPUT}${GUIEXEC}

-include ${OCAMLPATH}/Makefile.config
