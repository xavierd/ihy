CC=gcc

DEBUG=yes
ifeq ($(DEBUG),yes)
	CFLAGS=-g -Wall -W -pedantic
	OUTPUT=
else
	CFLAGS=-O2 -Wall -W -pedantic
	OUTPUT=../../bin/
endif

LDFLAGS=

OBJECTS=pcm.o main.o

EXEC=ihydeflate

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(OUTPUT)$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(OUTPUT)$(EXEC)
