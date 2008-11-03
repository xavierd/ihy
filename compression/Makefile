CC=gcc
CFLAGS=-O2 -Wall -W -pedantic
LDFLAGS=
EXEC=ihycreate
OBJECTS= pcm.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(CFLAGS) $(LDFLAGS)

pcm.o: pcm.c pcm.h

clean:
	rm -rf $(OBJECTS)
	rm -rf $(EXEC)
