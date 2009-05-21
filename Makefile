export DEBUG=false

MAKEFLAGS=--no-print-directory

all:
	@${MAKE} -C src/cli
	@${MAKE} -C src/gui
	@${MAKE} -C src/compare

clean:
	${MAKE} -C src/cli clean
	${MAKE} -C src/gui clean
	${MAKE} -C src/compare clean
