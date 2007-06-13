PREFIX := /usr/local

.PHONY: all tests examples install
all: examples

tests:
	$(MAKE) -k -C tests

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
