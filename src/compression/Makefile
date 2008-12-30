CC=gcc
CFLAGS=-Wall -W -pedantic

DEBUG=yes
ifeq ($(DEBUG),yes)
	CFLAGS+=-g
	OUTPUT=
else
	CFLAGS+=-O2
	OUTPUT=../../bin/
endif

LDFLAGS=

SRC=pcm.c ihy.c main.c
OBJECTS=${SRC:.c=.o}

EXEC=ihydeflate

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(OUTPUT)$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(OUTPUT)$(EXEC)
