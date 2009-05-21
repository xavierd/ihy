export DEBUG=false

all:
	${MAKE} -C src/cli

clean:
	${MAKE} -C src/cli clean
