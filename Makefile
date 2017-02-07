# Makefile
#
# Author: Suraj Narayanan S


# -O3 Optimize code (turns on all optimizations specified by -O2 and also turns on the -finline-functions, -funswitch-loops, -fpredictive-commoning, -fgcse-after-reload, -ftree-loop-vectorize, -ftree-slp-vectorize, -fvect-cost-model, -ftree-partial-pre and -fipa-cp-clone options).
# -D_GNU_SOURCE=1 means the compiler will use the GNU standard of compilation, the superset of all other standards under GNU C libraries.
# -D_REENTRANT causes the compiler to use thread safe (i.e. re-entrant) versions of several functions in the C library.
FLAGS := -O3
CXX := g++ -std=c++11
BIN_DIR := bin
OUT_FILE := aixi
MKDIR_P = mkdir -p

all: mk_bin mcAixiCtw

mk_bin:
	$(MKDIR_P) $(BIN_DIR)

mcAixiCtw: bin/agent.o bin/util.o bin/ContextTree.o bin/environment.o bin/search.o bin/main.o
	$(CXX) $(FLAGS) bin/agent.o bin/util.o bin/ContextTree.o bin/environment.o bin/search.o bin/main.o -o $(OUT_FILE)

bin/main.o: main.cpp
	$(CXX) $(FLAGS) -c main.cpp -o bin/main.o

bin/util.o: common/util.cpp
	$(CXX) $(FLAGS) -c common/util.cpp -o bin/util.o

bin/ContextTree.o: CTW/ContextTree.cpp
	$(CXX) $(FLAGS) -c CTW/ContextTree.cpp -o bin/ContextTree.o

bin/agent.o: AIXI/agent.cpp
	$(CXX) $(FLAGS) -c AIXI/agent.cpp -o bin/agent.o

bin/search.o: MCTS/search.cpp
	$(CXX) $(FLAGS) -c MCTS/search.cpp -o bin/search.o

bin/environment.o: environments/environment.cpp
	$(CXX) $(FLAGS) -c environments/environment.cpp -o bin/environment.o

clean:
	rm -rf ${OUT_FILE} bin/*.o
