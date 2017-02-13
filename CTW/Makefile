# Makefile
#
# Author: Suraj Narayanan S


# -O3 Optimize code (turns on all optimizations specified by -O2 and also turns on the -finline-functions, -funswitch-loops, -fpredictive-commoning, -fgcse-after-reload, -ftree-loop-vectorize, -ftree-slp-vectorize, -fvect-cost-model, -ftree-partial-pre and -fipa-cp-clone options).
# -D_GNU_SOURCE=1 means the compiler will use the GNU standard of compilation, the superset of all other standards under GNU C libraries.
# -D_REENTRANT causes the compiler to use thread safe (i.e. re-entrant) versions of several functions in the C library.
FLAGS := -O3
CXX := g++ -std=c++11
BIN_DIR := bin
OUT_FILE := ctwTest
MKDIR_P = mkdir -p

all: mk_bin ctw

mk_bin:
	$(MKDIR_P) $(BIN_DIR)

ctw: bin/CTNode.o bin/ContextTree.o bin/main.o
	$(CXX) $(FLAGS) bin/CTNode.o bin/ContextTree.o bin/main.o -o $(OUT_FILE)

bin/main.o: main.cpp
	$(CXX) $(FLAGS) -c main.cpp -o bin/main.o

bin/ContextTree.o: ContextTree.cpp
	$(CXX) $(FLAGS) -c ContextTree.cpp -o bin/ContextTree.o

bin/CTNode.o: CTNode.cpp
	$(CXX) $(FLAGS) -c CTNode.cpp -o bin/CTNode.o

clean:
	rm -rf ${OUT_FILE} bin/*.o
