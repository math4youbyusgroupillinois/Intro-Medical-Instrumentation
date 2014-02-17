TEXFILES=$(wildcard *.tex)
FILE=BME154L_S11_Exam1Tips

all:
	latex $(FILE)
	latex $(FILE)
	dvipdf $(FILE).dvi

clean:
	(rm -rf *.ps *.log *.dvi *.aux *.*% *.lof *.lop *.lot *.toc *.idx *.ilg *.ind *.bbl *blg)

spell:
	@for file in $(TEXFILES); do \
	    aspell check -x $$file; \
	done
