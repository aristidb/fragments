# based on http://www.c-plusplus.de/forum/viewtopic-var-t-is-88418.html
#

ifndef CXXFLAGS
CXXFLAGS := -g -W -Wall -Wno-long-long -pedantic -std=c++98 -I../include
endif
ifndef CXX
CXX      := g++
endif
ifndef LIBS
LIBS     :=
endif

SOURCES  := $(wildcard *.$(EXT))
DEPS     := $(patsubst %.$(EXT), %.dep, $(SOURCES))
TARGETS  := $(patsubst %.$(EXT), %, $(SOURCES))

.PHONY: all
all: $(TARGETS)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(TARGETS): %: %.$(EXT) %.dep
	$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@

$(DEPS): %.dep: %.$(EXT)
	$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.$(EXT)=.o) -o $@

.PHONY: clean
clean:
	rm -rf $(DEPS)

.PHONY: dist-clean
dist-clean:
	rm -rf $(DEPS) $(TARGETS)
