/******************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar, Baseline code Author
*******************************************************************************/

#include <cassert>
#include <iostream>
#include <math.h>

#include "../common/types.hpp"
#include "../common/constants.hpp"

#include "CTNode.hpp"
#include "ContextTree.hpp"

// create a context tree of specified maximum depth
ContextTree::ContextTree(uint_t depth) {
  rootNode = new CTNode();
  maxTreeDepth = depth;
}

ContextTree::~ContextTree() {
  if (rootNode)
    delete rootNode;
}

// clear the entire context tree
void ContextTree::clear() {
  sequenceHistory.clear();
  if (rootNode)
    delete rootNode;
  rootNode = new CTNode();
}

void ContextTree::update(const symbol_list_t& symbol_list) {
  for (symbol_t symbol : symbol_list)
    update(symbol);
}

void ContextTree::update(const symbol_t sym) {
  if (sequenceHistory.size() >= maxTreeDepth) {
    update(sym, 0, rootNode, NODE_UPDATE);
  }
  sequenceHistory.push_back(sym);
}

void ContextTree::update(const symbol_t sym,
                         uint_t depth,
                         CTNode* node,
                         const int node_action) {
  if (depth == maxTreeDepth) {
    node->updateLeaf(sym, node_action);
    return;
  }
  symbol_t childSym = sequenceHistory[sequenceHistory.size() - 1 - depth];
  if (!node->child(childSym)) {
    CTNode* child = new CTNode();
    node->addChild(childSym, child, node);
  }
  update(sym, depth + 1, node->child(childSym), node_action);
  node->update(sym, node_action);
}

/*
* Updates the history statistics, without touching the context tree
* use it to update the history of those percept bits that predict an action
* bit.
*/
void ContextTree::updateHistory(const symbol_list_t& symbol_list) {
  for (uint_t i = 0; i < symbol_list.size(); i++) {
    sequenceHistory.push_back(symbol_list[i]);
  }
}

// revert ContextTree with last symbol from history.
void ContextTree::revert() {
  const symbol_t lastSym = sequenceHistory.back();
  sequenceHistory.pop_back();
  if (sequenceHistory.size() >= maxTreeDepth - 1) {
    update(lastSym, 0, rootNode, NODE_REVERT);
  }
}

// shrinks the history down to a former size
void ContextTree::revertHistory(uint_t newsize) {
  assert(newsize <= sequenceHistory.size());
  while (sequenceHistory.size() > newsize)
    sequenceHistory.pop_back();
}

// Predict what the next symbol would be according to the context tree
// statistics.
symbol_t ContextTree::predictSymbol() {
  double logProbWeighted = rootNode->getLogProbWeighted();
  update(SYMBOL_1);
  double logProbWeightedPrime = rootNode->getLogProbWeighted();
  revert();
  double probOneGivenHistory = exp(logProbWeightedPrime - logProbWeighted);
  std::cout << "probOneGivenHistory: " << probOneGivenHistory << std::endl;
  if (probOneGivenHistory >= 0.5)
    return SYMBOL_1;
  return SYMBOL_0;
}

// generate a specified number of random symbols
// distributed according to the context tree statistics
void ContextTree::predictMutipleSymbols(symbol_list_t& predictedSymbols,
                                        uint_t numSymToPredict) {
  predictedSymbols.clear();
  for (uint_t i = 0; i < numSymToPredict; i++) {
    symbol_t predictedSymbol = predictSymbol();
    update(predictedSymbol);
    predictedSymbols.push_back(predictedSymbol);
  }
  // restore the context tree to it's original state
  for (uint_t i = 0; i < numSymToPredict; i++)
    revert();
}

// the logarithm of the block probability of the whole sequence
double ContextTree::getLogBlockProbability() {
  return rootNode->getLogProbWeighted();
}

// get the n'th most recent history symbol, NULL if doesn't exist
symbol_t* ContextTree::nthHistorySymbol(uint_t n) {
  return n < sequenceHistory.size() ? &sequenceHistory[n] : NULL;
}
