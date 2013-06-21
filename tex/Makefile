.PHONY: all clean

all: informe.pdf

informe.pdf: tasas-vs-k
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex

tasas-vs-k:
	cd ../octave; \
	octave --eval graficar_tasas; \
	mv tasas-vs-k.* ../tex

clean:
	rm -f *.pdf *.aux *.log *.fdb_latexmk *.fls