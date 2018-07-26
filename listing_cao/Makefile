SOURCES = listing_cao
BUILDDIR = build/
DEP=$(wildcard cao/*) assets/header.tex
TARGETS=$(addprefix $(BUILDDIR),$(addsuffix .pdf,$(SOURCES)))

# Change LaTeX engine and use custom header
PARAMETERS = --pdf-engine=xelatex
PARAMETERS += --include-in-header=assets/header.tex

# Set fonts
PARAMETERS += -V mainfont:"texgyrepagella.otf" \
	-V 'mainfontoptions: Path = fonts/, \
		UprightFont = *-regular, \
		BoldFont = *-bold, \
		ItalicFont = *-italic, \
		BoldItalicFont = *-bolditalic' \
	-V mathfont:"texgyrepagella-math.otf" \
	-V 'mathfontoptions: Path = fonts/'

all: $(TARGETS)

$(BUILDDIR)%.pdf : %.md $(DEP)
	@mkdir -p $(BUILDDIR) # Make sure build dir exists
	pandoc $(PARAMETERS) $< -o $@

clean:
	@rm -f $(TARGETS)

