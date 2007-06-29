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

SOURCES  := $(wildcard *.cpp)
DEPS     := $(patsubst %.cpp, %.dep, $(SOURCES))
TARGETS  := $(patsubst %.cpp, %, $(SOURCES))

.PHONY: all
all: $(TARGETS)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(TARGETS): %: %.cpp %.dep
	$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@

$(DEPS): %.dep: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.cpp=) -o $@

.PHONY: clean
clean:
	rm -rf $(DEPS)

.PHONY: dist-clean
dist-clean:
	rm -rf $(DEPS) $(TARGETS)
