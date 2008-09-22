
# XXX: The cut pipe is to strip the leading "./" from find results. It's
#      a little ugly but I couldn't find a better way to get a list of all
#      headers in the directory (recursively).
HEADERS = $(shell find -name '*.hpp' | cut -b3-)
HDRPATH = posixx


# install command
INSTALL = install -p -m 0644 -D


# install utility functions
install_msg = echo "  INSTALL $(1)"

install_files = @$(call install_msg,$(2)) && \
	for f in $(1); do \
		$(call install,$$f,$(2)/$$f); \
	done && \
	touch $@

ifneq ($(V), 1)
NICE_CLEAN = @echo "  CLEAN"; $(RM)
install = $(INSTALL) $(1) $(2)
else
NICE_CLEAN = $(RM) -v
install = $(call install_msg,$(2)); $(INSTALL) $(1) $(2)
endif


default: all

all:

install: install-headers

install-headers: .install-headers-stamp

.install-headers-stamp: $(HEADERS)
	$(call install_files,$^,$(prefix)/include/$(HDRPATH))

clean:
	$(NICE_CLEAN) .*-stamp

.PHONY: default all install install-headers clean

