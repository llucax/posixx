
# Build flags (include general stuff in the .pc file instead)
LOCAL_CFLAGS =
LOCAL_LDFLAGS =

# XXX: The cut pipe is to strip the leading "./" from find results. It's
#      a little ugly but I couldn't find a better way to get a list of all
#      headers in the directory (recursively).
HEADERS = $(shell find -name '*.hpp' | cut -b3-)
HDRPATH = posixx
PCFILE = posixx.pc
PCTEMPLATE = $(PCFILE).template

# prefix for installing the binaries
prefix = /usr/local


# Flags fetched from pkg-config .pc file
ifneq ($(PCTEMPLATE), )
PKG_LIB = $(shell grep Libs.private: $(PCTEMPLATE) | cut -d' ' -f2-)
PKG_REQ = $(shell grep Requires: $(PCTEMPLATE) | cut -d' ' -f2-)
ifneq ($(PKG_REQ), )
PKG_REQ_CFLAGS = $(shell pkg-config --cflags $(PKG_REQ))
PKG_REQ_LDFLAGS = $(shell pkg-config --libs $(PKG_REQ))
endif
endif

# XXX: LI_* variables are for internal use only
# (for the super-repository local-install target)
ALL_CFLAGS = $(LOCAL_CFLAGS) $(PKG_REQ_CFLAGS) $(LI_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(LOCAL_LDFLAGS) $(PKG_LIB) $(PKG_REQ_LDFLAGS) $(LI_LDFLAGS) \
		$(LDFLAGS)


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

$(PCFILE): $(PCTEMPLATE)
	@sed 's|PREFIX|$(prefix)|' $< > $@

install: install-headers install-pc

install-headers: .install-headers-stamp

.install-headers-stamp: $(HEADERS)
	$(call install_files,$^,$(prefix)/include/$(HDRPATH))

install-pc: .install-pc-stamp

.install-pc-stamp: $(PCFILE)
	$(call install_files,$^,$(prefix)/lib/pkgconfig)

clean:
	$(NICE_CLEAN) $(PCFILE) .*-stamp

.PHONY: default all install install-headers install-pc clean

