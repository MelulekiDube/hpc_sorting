#Makefile for running the program
#Meluleki Dube
#28 April 2019

CC := gcc 
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/runner

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall 
LIB :=  -fopenmp

INC := -I include
$(TARGET): $(OBJECTS)
	@echo " Linking...";
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) clean
	@mkdir -p $(BUILDDIR);
	@echo "compiling the source files"
	$(CC) $(LIB) $(CFLAGS) $(INC) -c -o $@ $<

run:
	./$(TARGET)
	
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET) $(BINDIR)

.PHONY: clean