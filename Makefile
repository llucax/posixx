

# prefix for installing the binaries and headers
prefix=/usr/local



default: all

all: test memtest

install:
	$(MAKE) -C src $@

doc:
	$(MAKE) -C doc $@

test memtest: all
	$(MAKE) -C test $@

clean:
	$(MAKE) -C src $@
	$(MAKE) -C doc $@
	$(MAKE) -C test $@

.PHONY: default all install doc test memtest clean


