

# prefix for installing the binaries and headers
prefix=/usr/local



default: all

all:

install:
	$(MAKE) -C src $@

doc:
	$(MAKE) -C doc $@

test memtest:
	$(MAKE) -C test $@

commit-check: doc test memtest

clean:
	$(MAKE) -C src $@
	$(MAKE) -C doc $@
	$(MAKE) -C test $@

.PHONY: default all install doc test memtest commit-check clean


