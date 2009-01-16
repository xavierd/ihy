export DEBUG=false

all:
	${MAKE} -C src/cli
	${MAKE} -C src/gui

clean:
	${MAKE} -C src/cli clean
	${MAKE} -C src/gui clean
