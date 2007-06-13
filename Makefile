

.PHONY: all tests examples
all: tests examples

tests:
	$(MAKE) -C tests

examples:
	$(MAKE) -C examples

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
