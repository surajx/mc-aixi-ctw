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

ContextTree::ContextTree(uint_t depth) {
  rootNode = new CTNode();
  maxTreeDepth = depth;
}

ContextTree::~ContextTree() {
  if (rootNode)
    delete rootNode;
}

void ContextTree::clear() {
  std::cout << "1: " << sequenceHistory.size()<< std::endl;
  sequenceHistory.clear();
  std::cout << "2: "<< sequenceHistory.size()<< std::endl;
  sequenceHistory.shrink_to_fit();
  std::cout << "3: "<< sequenceHistory.size()<< std::endl;
  sequenceHistory.resize(0);
  std::cout << "4: "<< sequenceHistory.size()<< std::endl;

  if (rootNode)
    deleteTree(rootNode);
    delete rootNode;
  rootNode = new CTNode();
}

void ContextTree::deleteTree(CTNode* node) {
  CTNode* child0 = node->child(SYMBOL_0);
  CTNode* child1 = node->child(SYMBOL_1);

  if (child0) {
    deleteTree(child0);
    delete child0;
  }
  if (child1) {
    deleteTree(child1);
    delete child1;
  }
}

void ContextTree::update(const symbol_list_t& symbol_list) {
  for (symbol_t symbol : symbol_list)
    update(symbol);
}

void ContextTree::update(const symbol_t sym) {
  // Don't update until a context is built-up
  if (sequenceHistory.size() >= maxTreeDepth) {
    update(sym, 0, rootNode, NODE_UPDATE);
  }
  sequenceHistory.push_back(sym);
}

void ContextTree::update(const symbol_t sym,
                         uint_t depth,
                         CTNode* node,
                         const int node_action) {
  // Recursion bottomed-out.
  if (depth == maxTreeDepth) {
    node->updateLeaf(sym, node_action);
    return;
  }

  // Get the next symbol in context.
  symbol_t childSym = sequenceHistory[sequenceHistory.size() - 1 - depth];

  // Create node for context if it doesnt exist.
  if (!node->child(childSym)) {
    CTNode* child = new CTNode();
    node->addChild(childSym, child, node);
  }

  // Recursively update each node.
  update(sym, depth + 1, node->child(childSym), node_action);

  // Actually update node on the way back after bottoming out.
  node->update(sym, node_action);
}

void ContextTree::updateHistory(const symbol_list_t& symbol_list) {
  for (uint_t i = 0; i < symbol_list.size(); i++) {
    sequenceHistory.push_back(symbol_list[i]);
  }
}

void ContextTree::revert() {
  // std::cout << "here1: " << std::endl;
  const symbol_t lastSym = sequenceHistory.back();
  // std::cout << "here2: " << std::endl;
  // std::cout << "lastSym: "<< lastSym << std::endl;
  // std::cout << "sequenceHistory.size b4: "<< sequenceHistory.size() << std::endl;
  // std::cout << "sequenceHistory.max_size b4: "<< sequenceHistory.max_size() << std::endl;  
  // std::cout << "sequenceHistory[5119]: "<< sequenceHistory[5119] << std::endl;
  sequenceHistory.pop_back();
  // std::cout << "sequenceHistory.size af: "<< sequenceHistory.size() << std::endl;
  // std::cout << "sequenceHistory.max_size af: "<< sequenceHistory.max_size() << std::endl;  
  // std::cout << "here3: " << std::endl;
  if (sequenceHistory.size() >= maxTreeDepth - 1) {
    update(lastSym, 0, rootNode, NODE_REVERT);
  }
  // std::cout << "here4: " << std::endl;
}

void ContextTree::revertHistory(uint_t newsize) {
  assert(newsize <= sequenceHistory.size());
  while (sequenceHistory.size() > newsize)
    sequenceHistory.pop_back();
}

double ContextTree::predict(symbol_t sym) {
  double logProbWeighted = rootNode->getLogProbWeighted();
  update(sym);
  double logProbWeightedPrime = rootNode->getLogProbWeighted();
  revert();
  return exp(logProbWeightedPrime - logProbWeighted);
}

double ContextTree::predict(symbol_list_t& symbol_list) {
  double logProbWeighted = rootNode->getLogProbWeighted();
  for (uint_t i = 0; i < symbol_list.size(); i++) {
    update(symbol_list[i]);
  }
  double logProbWeightedPrime = rootNode->getLogProbWeighted();
  for (uint_t i = 0; i < symbol_list.size(); i++) {
    revert();
  }
  return exp(logProbWeightedPrime - logProbWeighted);
}

symbol_t ContextTree::predictSymbol() {
  double probOneGivenHistory = predict(SYMBOL_1);
  if (probOneGivenHistory >= 0.5)
    return SYMBOL_1;
  return SYMBOL_0;
}

void ContextTree::genNextSymbols(symbol_list_t& predictedSymbols, uint_t num) {
  genNextSymbolsAndUpdate(predictedSymbols, num);
  // Restore the context tree to it's original state.
  for (uint_t i = 0; i < num; i++)
    revert();
}

void ContextTree::genNextSymbolsAndUpdate(symbol_list_t& predictedSymbols,
                                          uint_t num) {
  predictedSymbols.clear();
  for (uint_t i = 0; i < num; i++) {
    // std::cout << "cnt: " << i << std::endl;
    symbol_t predictedSymbol = predictSymbol();
    update(predictedSymbol);    
    predictedSymbols.push_back(predictedSymbol);    
  }
}

double ContextTree::getLogBlockProbability() {
  return rootNode->getLogProbWeighted();
}

symbol_t* ContextTree::nthHistorySymbol(uint_t n) {
  return n < sequenceHistory.size() ? &sequenceHistory[n] : NULL;
}
