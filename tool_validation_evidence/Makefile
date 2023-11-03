#================================================================
# Specify your tex source files here.

sources := entropy_report_biased-random-bits_x64_AMD.tex \
entropy_report_biased-random-bytes_x64_AMD.tex \
entropy_report_data.pi_x64_AMD.tex \
entropy_report_rand1_short_x64_AMD.tex \
entropy_report_rand4_short_x64_AMD.tex \
entropy_report_rand8_short_x64_AMD.tex \
entropy_report_ringOsc-nist_x64_AMD.tex \
entropy_report_truerand_1bit_x64_AMD.tex \
entropy_report_truerand_4bit_x64_AMD.tex \
entropy_report_truerand_8bit_x64_AMD.tex \
entropy_report_normal_x64_AMD.tex
pdfs := $(subst .tex,.pdf, $(sources))
logs := $(subst .tex,.log, $(sources))
auxs := $(subst .tex,.aux, $(sources))

TEX=C:\usr\texlive\2023\bin\windows\xelatex
MAKEINDEX=mendex
.PHONY:	all single double triple clean

define compile-triple
	$(eval pdf:=$(subst .tex,.pdf, $1))
	$(TEX) $1					
	$(rm -f $(pdf))				
	$(TEX) $1					
	$(rm -f $(pdf))				
	$(TEX) $1					
endef

$(pdfs) : $(sources)
	$(foreach src,$(sources),$(call compile-triple,$(src)))

#================================================================
# usually, you don't have to edit the following lines.

.SUFFIXES:
.SUFFIXES:	.tex .aux .bbl .ind .pdf
#.tex.pdf:
#	$(TEX) $*
#.aux.bbl:
#	$(BIBTEX) $*
#.aux.ind:
#	$(MAKEINDEX) $*
#
#%.pdf: %.tex
#	$(TEX) $<
#
#
single: $(pdfs)
#
#double: single
#	$(foreach pdf,$(pdfs),$(rm -f $(pdf)))
#	$(MAKE) single
#
#triple: double
#	rm -f $(pdfs)
#	$(MAKE) double

#pdf: $(MAIN).pdf

all: $(pdfs)

clean:
	$(foreach pdf,$(pdfs),$(rm -f $(pdf)))
	$(foreach log,$(logs),$(rm -f $(log)))
	$(foreach aux,$(auxs),$(rm -f $(aux)))

distclean: clean
	rm -f $(pdfs) $(logs) $(auxs)

veryclean: distclean

$(MAIN).pdf $(MAIN).aux: $(SRCS)
	$(TEX) $(MAIN).tex