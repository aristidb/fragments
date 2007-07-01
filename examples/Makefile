# based on http://www.c-plusplus.de/forum/viewtopic-var-t-is-88418.html
#

CXXFLAGS ?= -g -W -Wall -Wno-long-long -pedantic -std=c++98 -I../include
CXX      ?= g++
LIBS     ?=

SOURCES  := $(wildcard *.cpp)
DEPS     := $(patsubst %.cpp, %.dep, $(SOURCES))
TARGETS  := $(patsubst %.cpp, %, $(SOURCES))

.PHONY: all
all: $(TARGETS)

# argl! GNU/Make does not support boolean algebra
# thus a hack like this is required to avoid including deps in clean, dist-clean
ifeq ($(MAKECMDGOALS), clean)
else
ifneq ($(MAKECMDGOALS), dist-clean)
-include $(DEPS)
endif
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
