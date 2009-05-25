export DEBUG=false

MAKEFLAGS=--no-print-directory
PREFIX=/usr/local

all:
	@${MAKE} -C src/cli
	@${MAKE} -C src/compare

install:
	cp bin/ihycli ${PREFIX}/bin
	cp bin/ihygui ${PREFIX}/bin

uninstall:
	rm ${PREFIX}/bin/ihycli
	rm ${PREFIX}/bin/ihygui

clean:
	${MAKE} -C src/cli clean
	${MAKE} -C src/gui clean
	${MAKE} -C src/compare clean
