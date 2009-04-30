

# prefix for installing the binaries and headers
prefix=/usr/local



default: all

all:

install:
	$(MAKE) -C src install

doc:
	$(MAKE) -C doc

test:
	$(MAKE) -C test test

memtest:
	$(MAKE) -C test memtest

commit-check: doc test memtest

clean:
	$(MAKE) -C src clean
	$(MAKE) -C doc clean
	$(MAKE) -C test clean

.PHONY: default all install doc test memtest commit-check clean


