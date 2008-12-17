DEBUG=no
CC=gcc
ifeq ($(DEBUG),yes)
	CFLAGS=-g -Wall -W -pedantic
else
	CFLAGS=-O2 -Wall -W -pedantic
endif
LDFLAGS=
EXEC=ihycreate
OBJECTS= pcm.o main.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(CFLAGS) $(LDFLAGS)

pcm.o: pcm.c pcm.h

clean:
	rm -rf $(OBJECTS)
	rm -rf $(EXEC)
