# Makefile
#
# Author: Suraj Narayanan Sasikumar


# -O3 Optimize code (turns on all optimizations specified by -O2 and also turns on the -finline-functions, -funswitch-loops, -fpredictive-commoning, -fgcse-after-reload, -ftree-loop-vectorize, -ftree-slp-vectorize, -fvect-cost-model, -ftree-partial-pre and -fipa-cp-clone options).
# -D_GNU_SOURCE=1 means the compiler will use the GNU standard of compilation, the superset of all other standards under GNU C libraries.
# -D_REENTRANT causes the compiler to use thread safe (i.e. re-entrant) versions of several functions in the C library.
# FLAGS := -g # Debug
FLAGS := -O3
CXX := g++ -std=c++11
BIN_DIR := bin
OUT_FILE := aixi
MKDIR_P = mkdir -p

all: mk_bin mcAixiCtw

mk_bin:
	$(MKDIR_P) $(BIN_DIR)

mcAixiCtw: bin/agent.o bin/util.o bin/CTNode.o bin/ContextTree.o bin/environment.o bin/search.o bin/main.o
	$(CXX) $(FLAGS) $(BIN_DIR)/agent.o $(BIN_DIR)/util.o $(BIN_DIR)/CTNode.o $(BIN_DIR)/ContextTree.o $(BIN_DIR)/environment.o $(BIN_DIR)/search.o $(BIN_DIR)/main.o -o $(BIN_DIR)/$(OUT_FILE)

bin/main.o: main.cpp
	$(CXX) $(FLAGS) -c main.cpp -o $(BIN_DIR)/main.o

bin/util.o: common/util.cpp
	$(CXX) $(FLAGS) -c common/util.cpp -o $(BIN_DIR)/util.o

bin/ContextTree.o: CTW/ContextTree.cpp
	$(CXX) $(FLAGS) -c CTW/ContextTree.cpp -o $(BIN_DIR)/ContextTree.o

bin/CTNode.o: CTW/CTNode.cpp
	$(CXX) $(FLAGS) -c CTW/CTNode.cpp -o $(BIN_DIR)/CTNode.o

bin/agent.o: AIXI/agent.cpp
	$(CXX) $(FLAGS) -c AIXI/agent.cpp -o $(BIN_DIR)/agent.o

bin/search.o: MCTS/search.cpp
	$(CXX) $(FLAGS) -c MCTS/search.cpp -o $(BIN_DIR)/search.o

bin/environment.o: environments/environment.cpp
	$(CXX) $(FLAGS) -c environments/environment.cpp -o $(BIN_DIR)/environment.o

clean:
	rm -rf $(BIN_DIR)/${OUT_FILE} $(BIN_DIR)/*.o
