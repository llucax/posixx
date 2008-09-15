

# prefix for installing the binaries and headers
prefix=/usr/local


ifneq ($(V), 1)
NICE_DOXY = @echo "  DOXYGEN"; (cat Doxyfile; echo "QUIET=YES") | doxygen -
NICE_CLEAN = @echo "  CLEAN"; $(RM)
NICE = @
else
NICE_DOXY = doxygen
NICE_CLEAN = $(RM) -v
NICE =
endif


default: all

all: doc

doc: include
	$(NICE_DOXY)

install: install-man install-html

install-man: doc
	$(NICE) for f in `find reference/man -type f | cut -b11-`; do \
		echo "  INSTALL $(prefix)/share/$$f"; \
		install -m 0644 -D reference/$$f $(prefix)/share/$$f; \
	done

install-html: doc
	$(NICE) for f in `find reference/html -type f | cut -b11-`; do \
		echo "  INSTALL $(prefix)/share/doc/posixx/$$f"; \
		install -m 0644 -D reference/$$f $(prefix)/share/doc/posixx/$$f; \
	done

include:
	@mkdir -p include
	@ln -fs ../../src include/posixx

clean:
	$(NICE_CLEAN) -r reference include

.PHONY: default all doc install install-man install-doc clean


