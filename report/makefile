# Makefile to create pdf (and ps) from tex files.
# DON'T FORGET TO PUT THE NAME IN THIS VARIABLE
NAME=project

default : compile

# Clean target, removes everything except tex file
clean :
	$(RM)		$(NAME).aux
	$(RM)		$(NAME).bbl
	$(RM)		$(NAME).blg
	$(RM)		$(NAME).dvi
	$(RM)		$(NAME).log
	$(RM)		$(NAME).out
	$(RM)		$(NAME).pdf
	$(RM)		$(NAME).ps
	$(RM)		$(NAME).tpt

# Default target, creates the aux and pdf files. Aux file is needed by bibtex.
# Bibtex is out commented here, uncomment to enable.
compile :
	latex		$(NAME)	# Make aux that bibtex need.
	#bibtex 		$(NAME) # Make bibtex.
	latex		$(NAME) # Update aux after bibtex ran.
	pdflatex	$(NAME)	# Make aux that bibtex need.

# Make a postscript file
ps: compile
	dvips -o	$(NAME).ps $(NAME).dvi
