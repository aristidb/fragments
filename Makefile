

.PHONY: all tests examples
all: tests examples

tests:
	cd tests && make

examples:
	cd tests && make

.PHONY: clean tests-clean examples-clean
clean: tests-clean examples-clean

tests-clean:
	cd tests && make clean

examples-clean:
	cd tests && make clean

.PHONY: dist-clean tests-dist-clean examples-dist-clean
dist-clean: tests-dist-clean examples-dist-clean

tests-dist-clean:
	cd tests && make dist-clean

examples-dist-clean:
	cd tests && make dist-clean
