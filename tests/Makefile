# based on http://www.c-plusplus.de/forum/viewtopic-var-t-is-88418.html
#

# TODO use colors?
ifndef CXXFLAGS
CXXFLAGS := -g -W -Wall -Wno-long-long -pedantic -std=c++98 -I../include
endif
ifndef CXX
CXX      := g++
endif
ifndef LIBS
LIBS     :=
endif

SOURCES       := $(wildcard *.cpp)
DEPS          := $(patsubst %.cpp, %.dep, $(SOURCES))

COMPSOURCES   := $(wildcard *-valid.cpp)
COMPTARGETS   := $(patsubst %.cpp, %, $(COMPSOURCES))

NOCOMPSOURCES := $(wildcard *-invalid.cpp)
NOCOMPTARGETS := $(patsubst %.cpp, %, $(NOCOMPSOURCES))

.PHONY: all test start
all:
	@$(MAKE) -k test

test: start $(NOCOMPTARGETS) $(COMPTARGETS)
	@echo "Done"

start:
	@echo "Running Tests:"

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(COMPTARGETS): %: %.cpp %.dep
	@if $(CXX) $(CXXFLAGS) $(LIBS) $< -o $@; then \
	  if ./$@; then \
	    echo -e '\t' $< 'passed'; \
	    true; \
	  else \
	    echo -e '\t' $< 'failed [running]'; \
	    false; \
	  fi; \
	 else  \
	  echo -e '\t' $< 'failed [compile]'; \
	  false; \
	 fi

$(DEPS): %.dep: %.cpp
	@$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.cpp=) -o $@

$(NOCOMPTARGETS): %: %.cpp %.dep
	@if $(CXX) $(CXXFLAGS) $(LIBS) $< -o $@ 2> /dev/null; then \
	   echo -e '\t' $< 'failed'; \
	   rm -f $@; \
           false; \
         else \
           echo -e '\t' $< 'passed'; \
	   touch $@; \
	   true; \
         fi

.PHONY: clean
clean: dist-clean

.PHONY: dist-clean
dist-clean:
	@rm -rf $(DEPS) $(COMPTARGETS) $(NOCOMPTARGETS)

.PHONY: rerun
rerun: dist-clean all
