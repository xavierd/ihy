filename=ihy

all:pdf clean

pdf:
	pdflatex "${filename}.tex"
	pdflatex "${filename}.tex"

clean:
	rm -f "${filename}.aux" "${filename}.log" "${filename}.toc" "texput.log"

mrproper: clean
	rm -f "${filename}.pdf"
