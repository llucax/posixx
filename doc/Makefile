
# sources from where to build the sources
DOC_SOURCES = $(shell find ../src -name '*.hpp')

# prefix for installing the binaries and headers
prefix=/usr/local


# install command
INSTALL = install -p -m 0644 -D


# install utility functions
install_msg = echo "  INSTALL $(1)"

install_group = @$(call install_msg,$(2)) && \
	cd reference && \
	for f in `find $(1) -type f`; do \
		$(call install,$$f,$(2)/$$f); \
	done && \
	cd .. && \
	touch $@

ifneq ($(V), 1)
NICE_DOXY = @echo "  DOXYGEN"; (cat Doxyfile; echo "QUIET=YES") | doxygen -
NICE_CLEAN = @echo "  CLEAN"; $(RM)
install = $(INSTALL) $(1) $(2)
else
NICE_DOXY = doxygen
NICE_CLEAN = $(RM) -v
install = $(call install_msg,$(2)); $(INSTALL) $(1) $(2)
endif


default: all

all: doc

doc: .doc-stamp

.doc-stamp: include $(DOC_SOURCES) Doxyfile
	$(NICE_DOXY)
	@touch $@

install: install-man install-html

install-man: .install-man-stamp

.install-man-stamp: .doc-stamp
	$(call install_group,man,$(prefix)/share/man)

install-html: .install-html-stamp

.install-html-stamp: .doc-stamp
	$(call install_group,html,$(prefix)/share/doc/posixx/html)

include:
	@mkdir -p include
	@ln -fs ../../src include/posixx

clean:
	$(NICE_CLEAN) -r reference include .*-stamp

.PHONY: default all doc install install-man install-doc clean

