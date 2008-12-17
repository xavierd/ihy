CC=gcc

DEBUG=no
ifeq ($(DEBUG),yes)
	CFLAGS=-g -Wall -W -pedantic
else
	CFLAGS=-O2 -Wall -W -pedantic
endif

LDFLAGS=

OBJECTS=pcm.o main.o

OUTPUT=../../bin/
EXEC=ihydeflate

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(OUTPUT)$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(OUTPUT)$(EXEC)
