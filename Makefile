PREFIX := /usr/local
export PREFIX

CXXFLAGS := -g -W -Wall -Wno-long-long -pedantic -std=c++98 -I../include
export CXXFLAGS
CXX      := g++
export CXX
LIBS     :=
export LIBS

.PHONY: all tests examples install
all: examples

tests:
	@$(MAKE) -k -C tests rerun

examples:
	$(MAKE) -C examples

install:
	@echo "TODO" #@cp -r include/fragments $(PREFIX)/include

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
