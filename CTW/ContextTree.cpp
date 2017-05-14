/******************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar, Baseline code Author
*******************************************************************************/

#include <cassert>
#include <iostream>
#include <math.h>

#include "../common/util.hpp"
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
  sequenceHistory.clear();
  if (rootNode) {
    // TODO: Investigate further why this results in a strange unconsequential
    // segfault.
    // deleteTree(rootNode);
    delete rootNode;
  }
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
    if (node_action == NODE_REVERT && node->visits() == 0) {
      node->getParent()->nullifyChild(node);
      delete node;
      node = NULL;
    }
    return;
  }

  // Get the next symbol in context.
  symbol_t childSym = sequenceHistory[sequenceHistory.size() - 1 - depth];

  // Create node for context if it doesnt exist.
  if (node->child(childSym) == NULL) {
    CTNode* child = new CTNode();
    node->addChild(childSym, child, node);
  }

  // Recursively update each node.
  update(sym, depth + 1, node->child(childSym), node_action);

  // Actually update node on the way back after bottoming out.
  node->update(sym, node_action);
  if (node_action == NODE_REVERT && node->visits() == 0) {
    node->getParent()->nullifyChild(node);
    delete node;
    node = NULL;
  }
}

void ContextTree::updateHistory(const symbol_list_t& symbol_list) {
  for (uint_t i = 0; i < symbol_list.size(); i++) {
    sequenceHistory.push_back(symbol_list[i]);
  }
}

void ContextTree::revert() {
  // CTW history should always have size >= depth
  assert(
      "Revert called on CTW which makes history size less than tree depth: " &&
      sequenceHistory.size() >= maxTreeDepth + 1);
  const symbol_t lastSym = sequenceHistory.back();
  sequenceHistory.pop_back();
  update(lastSym, 0, rootNode, NODE_REVERT);
}

void ContextTree::revertHistory(uint_t newsize) {
  assert(
      "New reverted history size should be less than current history size: " &&
      newsize <= sequenceHistory.size());
  while (sequenceHistory.size() > newsize)
    sequenceHistory.pop_back();
}

void ContextTree::revertHistory() {
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

void ContextTree::printCurrentContext() {
  std::cout << "Context: ";
  for (int i = sequenceHistory.size() - maxTreeDepth;
       i < sequenceHistory.size(); i++) {
    std::cout << "@" << sequenceHistory.size() - i << ": " << sequenceHistory[i]
              << ", ";
  }
  std::cout << std::endl;
}

symbol_t ContextTree::predictSymbol() {
  // std::cout << "Next symbol 1 prob: " << predict(SYMBOL_1) << std::endl;
  return predict(SYMBOL_1) > 0.5 ? SYMBOL_1 : SYMBOL_0;
}

void ContextTree::genNextSymbols(symbol_list_t& predictedSymbols, uint_t num) {
  genNextSymbolsAndUpdate(predictedSymbols, num);
  // Restore the context tree to it's original state.
  for (uint_t i = 0; i < num; i++) {
    revert();
  }
}

void ContextTree::genNextSymbolsAndUpdate(symbol_list_t& predictedSymbols,
                                          uint_t num) {
  predictedSymbols.clear();
  symbol_t predictedSymbol;
  for (uint_t i = 0; i < num; i++) {
    predictedSymbol = predictSymbol();
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
