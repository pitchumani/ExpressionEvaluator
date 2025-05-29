CXX = clang++
CXXFLAGS = -std=c++17 -I.. -I./ -Wall -Wextra -pthread

# List all test source files and main library sources
SRCS = main.cpp test_expression.cpp test_parser.cpp ../expression.cpp ../parser.cpp
OBJS = $(SRCS:.cpp=.o)

# Google Test (assumes installed system-wide, e.g., via brew or apt)
LIBS = -lgtest -lgtest_main -pthread

TARGET = runTests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: all
	./$(TARGET)