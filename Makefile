PREFIX ?= /usr/local
export PREFIX

-include VERSION
VERSION ?= 1.0

INCLUDEDIR := $(PREFIX)/include/fragments-$(VERSION)

BOOSTDIR = /usr/local/include/boost-1_37

CXXFLAGS := -g -W -Wall -Wno-long-long -pedantic -std=c++98 -I../include -I$(BOOSTDIR)
export CXXFLAGS
CXX      := g++
export CXX
LIBS     :=
export LIBS

.PHONY: all tests examples install uninstall
all: examples

tests:
	@$(MAKE) -k -C tests rerun

examples:
	$(MAKE) -C examples

install:
# should we use install(1) instead of cp?
	@echo "Installing Fragments $(VERSION) to $(INCLUDEDIR)"
	@mkdir -p $(INCLUDEDIR)
	@cp -r include/fragments $(INCLUDEDIR)

uninstall:
	@echo "Uninstalling Fragments $(VERSION) from $(INCLUDEDIR)"
	@rm -rf $(INCLUDEDIR)

.PHONY: clean tests-clean examples-clean
clean: tests-clean examples-clean

tests-clean:
	$(MAKE) -C tests clean

examples-clean:
	$(MAKE) -C examples clean

.PHONY: dist-clean tests-dist-clean examples-dist-clean
dist-clean: tests-dist-clean examples-dist-clean

tests-dist-clean:
	$(MAKE) -C tests dist-clean

examples-dist-clean:
	$(MAKE) -C examples dist-clean

.PHONY: dist
dist:
	@if [ -e make-dist.sh ]; then ./make-dist.sh; fi

.PHONY: help
help:
	@cat INSTALL
