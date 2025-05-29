.DEFAULT_GOAL := all

CXX=clang++
COMPILE_FLAGS=$(CXXFLAGS) -g3 -std=c++17
GTEST_FLAGS=-lgtest


all: main.cpp expression.cpp parser.cpp
	$(CXX) $^ $(COMPILE_FLAGS) -o eval

clean:
	rm eval

