.PHONY: all clean graficos tasas-vs-k tasas-vs-iteraciones

all: informe.pdf

informe.pdf:
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex

graficos: tasas-vs-k tasas-vs-iteraciones

tasas-vs-k:
	cd ../octave; \
	octave --eval graficar_tasas_vs_k; \
	mv tasas-vs-k.tex tasas-vs-k.eps ../tex

tasas-vs-iteraciones:
	cd ../octave; \
	octave --eval graficar_tasas_vs_iteraciones; \
	mv tasas-vs-iteraciones.tex tasas-vs-iteraciones.eps ../tex

clean:
	rm -f *.pdf *.aux *.log *.fdb_latexmk *.fls