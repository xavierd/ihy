all:
	${MAKE} -C src/compression
#	${MAKE} -C src/decompression
#	${MAKE} -C src/reader

clean:
	${MAKE} -C src/compression clean
#	${MAKE} -C src/decompression clean
#	${MAKE} -C src/reader clean
