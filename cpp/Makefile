.DEFAULT_GOAL := all
.PHONY: all test runtest clean

# Makefile for evaluating expressions and running tests using Google Test
# Ensure you have Google Test installed and the paths are set correctly

CXX=clang++
COMPILE_FLAGS=$(CXXFLAGS) -g3 -std=c++17

SRCS := main.cpp expression.cpp parser.cpp
HDRS := expression.h parser.h

BUILDDIR := build
TARGET := $(BUILDDIR)/eval

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS) | $(BUILDDIR)
	$(CXX) $(COMPILE_FLAGS) $(SRCS) -o $@

unittest: $(BUILDDIR)
	$(MAKE) -C ./googletest test

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(MAKE) -C ./googletest clean
	rm -rf $(BUILDDIR)
