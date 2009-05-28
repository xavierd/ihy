export DEBUG=false

MAKEFLAGS=--no-print-directory
PREFIX=/usr/local

all:
	${MAKE} -C src/cli

clean:
	${MAKE} -C src/cli clean
