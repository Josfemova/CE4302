CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++23 -Wall -Wextra
LDFLAGS=-g 
LDLIBS=   

IDIR=src
SRCDIR=src
ODIR=build

# Headers dentro de include/
DEPS = $(shell find $(IDIR)/ -name "*.hpp")
SRC = $(shell find $(SRCDIR)/ -name "*.cpp")
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(ODIR)/%.o, $(SRC))

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

simul: $(OBJ)
	$(CXX) -o $(ODIR)/$@ $^ $(CPPFLAGS) $(LDLIBS)

.PHONY: clean install

clean:
	rm -rf $(ODIR)
