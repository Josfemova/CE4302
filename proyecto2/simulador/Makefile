CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++20 -I/opt/homebrew/include  # Include path for libwebsockets
LDFLAGS=-g -L/opt/homebrew/lib  # Library path for libwebsockets
LDLIBS=-lwebsockets  # Link the websockets library

IDIR=src
SRCDIR=src
ODIR=build

# Find all header files
DEPS = $(shell find $(IDIR)/ -name "*.hpp")

# Find all source files and exclude test files
SRC = $(shell find $(SRCDIR) -name "*.cpp" ! -path "$(SRCDIR)/tests/*")
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(ODIR)/%.o, $(SRC))

# Find all test source files
TEST_SRC = $(shell find $(SRCDIR)/tests -name "*.cpp")
TEST_OBJ = $(patsubst $(SRCDIR)/tests/%.cpp, $(ODIR)/tests/%.o, $(TEST_SRC))

# Collect object files for tests
CPU_OBJ_TESTS = $(ODIR)/CPU.o  
COMPILER_OBJ_TESTS = $(ODIR)/Compiler.o  

# Rule for compiling source files (excluding tests)
$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

# Rule for compiling test files
$(ODIR)/tests/%.o: $(SRCDIR)/tests/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

# Main application target
simul: $(OBJ)
	$(CXX) -o $(ODIR)/$@ $^ $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

# Test target
tests: $(TEST_OBJ) $(CPU_OBJ_TESTS) ${COMPILER_OBJ_TESTS}  # Include CPU_OBJ_TESTS for linking
	$(CXX) -o $(ODIR)/tests/tests $^ $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

# Phony targets
.PHONY: clean install tests simul

clean:
	$(RM) -rf $(ODIR)
