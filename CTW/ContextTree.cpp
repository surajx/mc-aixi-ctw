/******************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar, Baseline code Author
*******************************************************************************/

#include <cassert>

#include "ContextTree.hpp"
#include "../common/types.hpp"
#include "../common/constants.hpp"

/**
* Context Tree Node implementation.
*/

CTNode::CTNode(){
  logProbEstimated = 0.0;
  logProbWeighted = 0.0;
  symbolCount[SYMBOL_0] = 0;
  symbolCount[SYMBOL_1] = 0;
}

CTNode::~CTNode() {
  for (CTNode* child : children) {
    delete child;
  }
}

void CTNode::addChild(symbol_t sym, CTNode* child, CTNode* parent) {
  children[sym] = child;
  child->parent = parent;
}

uint_t CTNode::descendentsCount() {
  uint_t numDescendants = 1;  // Including current Node?
  numDescendants += child(SYMBOL_0) ? child(SYMBOL_0)->descendentsCount() : 0;
  numDescendants += child(SYMBOL_1) ? child(SYMBOL_1)->descendentsCount() : 0;
  return numDescendants;
}

// compute the logarithm of the KT-estimator update multiplier
double CTNode::logKTMul(symbol_t sym) {
  return NULL;  // TODO: implement
}

// Calculate the logarithm of the weighted block probability.
void CTNode::updateLogProbability(void) {
  // TODO : implement
}

// Update the node after having observed a new symbol.
void CTNode::update(const symbol_t symbol) {
  // TODO : implement
}

// Revert after MCTS look-ahead simulation is done.
void CTNode::revert(const symbol_t symbol) {
  // TODO : implement
}


/**
* Context Tree implementation.
*/

// create a context tree of specified maximum depth
ContextTree::ContextTree(uint_t depth){
  rootNode = new CTNode();
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

void ContextTree::update(const symbol_t sym) {
  // TODO: implement the actual update of context tree.
  sequenceHistory.push_back(sym);
}

void ContextTree::update(const symbol_list_t& symbol_list) {
  for(symbol_t symbol : symbol_list)
    update(symbol);
}

// updates the history statistics, without touching the context tree
// use it to update the history of those percept bits that predict an action
// bit.
void ContextTree::updateHistory(const symbol_list_t& symbol_list) {
  for (size_t i = 0; i < symbol_list.size(); i++) {
    sequenceHistory.push_back(symbol_list[i]);
  }
}

// removes the most recently observed symbol from the context tree
void ContextTree::revert() {
  // TODO: implement reverting a symbol
}

// shrinks the history down to a former size
void ContextTree::revertHistory(uint_t newsize) {

    assert(newsize <= sequenceHistory.size());
    while (sequenceHistory.size() > newsize) sequenceHistory.pop_back();
}

// Predict what the next symbol would be according to the context tree
// statistics.
symbol_t ContextTree::predictSymbol() {
  // TODO: actually predict looking at root node weighted Prob
  return true;  // Dummy symbol_t type
}

// generate a specified number of random symbols
// distributed according to the context tree statistics
void ContextTree::predictMutipleSymbols(uint_t numSymToPredict,
                                        symbol_list_t& predictedSymbols) {
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
