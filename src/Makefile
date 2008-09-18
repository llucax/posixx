
# XXX: The cut pipe is to strip the leading "./" from find results. It's
#      a little ugly but I couldn't find a better way to get a list of all
#      headers in the directory (recursively).
HEADERS = $(shell find -name '*.hpp' | cut -b3-)
HDRPATH = posixx


ifneq ($(V), 1)
NICE_NOP =
NICE = @
else
NICE_NOP = @echo "Nothing to do, headers only"
NICE =
endif


default: all

all:
	$(NICE_NOP)

install-headers: $(HEADERS)
	$(NICE) for f in $(HEADERS); do \
		echo "  INSTALL $(prefix)/include/$(HDRPATH)/$$f"; \
		install -p -m 0644 -D $$f $(prefix)/include/$(HDRPATH)/$$f; \
	done

install: install-headers

clean:
	$(NICE_NOP)

.PHONY: default all clean install-headers install

