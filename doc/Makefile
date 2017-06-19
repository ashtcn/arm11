EXTRA_SRC =
TEX = pdflatex

all: Report.pdf Checkpoint.pdf Presentation.pdf
	make -C LaTeX\ Documentation

.phony: all clean

%.pdf: %.tex $(EXTRA_SRC)
	while( $(TEX) $< ; grep -q "Rerun to get cross" $(<:.tex=.log)) ; \
	  do true ; \
	 done

clean:
	rm -f *.pdf
	rm -f *.aux
	rm -f *.log
	rm -f *.synctex.gz
	make clean -C LaTeX\ Documentation
