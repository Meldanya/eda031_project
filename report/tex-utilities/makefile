TMPFOLD		= ./tmp
TEXFLAGS	= -shell-escape -output-directory $(TMPFOLD)
TEXOBJS		= report
BIBOBJS		= refs.bib
TEX		= pdflatex
BIB		= bibtex

all : $(OBJS) $(BIBOBJS)
	$(TEX) $(TEXFLAGS) $(TEXOBJS)
	$(BIB) $(TMPFOLD)/$(TEXOBJS)
	$(TEX) $(TEXFLAGS) $(TEXOBJS)
	$(TEX) $(TEXFLAGS) $(TEXOBJS)
	detex $(TEXOBJS) | wc