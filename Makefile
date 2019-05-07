#Makefile for running the program
#Meluleki Dube
#28 April 2019

CC := gcc
MPICC := mpicc
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/runner

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES := $(filter-out $(SRCDIR)/mpi_sort.c, $(SOURCES))
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
	
	
mpi_sort:
	@mkdir -p $(BINDIR);
	@echo "compiling with mpicc"
	$(MPICC) $(INC) $(SRCDIR)/mpi_sort.c $(SRCDIR)/serial_qsort.c $(SRCDIR)/base.c -o $(BINDIR)/mpi_sort.out -lm 
	
run:
	./$(TARGET)
	
mpi_sort_run: mpi_sort
	mpirun -np 4 ./$(BINDIR)/mpi_sort.out
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET) $(BINDIR)

.PHONY: clean