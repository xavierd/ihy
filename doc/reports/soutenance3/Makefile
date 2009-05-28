filename=soutenance3

all:pdf clean

pdf:
	pdflatex "${filename}.tex"
	pdflatex "${filename}.tex"

clean:
	rm -f *.aux *.log *.toc *.log

mrproper: clean
	rm -f "${filename}.pdf"
