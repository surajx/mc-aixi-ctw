/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/

#include <math.h>
#include <cassert>

#include "../common/types.hpp"
#include "../common/constants.hpp"

#include "CTNode.hpp"

CTNode::CTNode() {
  logProbKT = 0.0;
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
  uint_t numDescendants = 1;  // Including current Node
  numDescendants += child(SYMBOL_0) ? child(SYMBOL_0)->descendentsCount() : 0;
  numDescendants += child(SYMBOL_1) ? child(SYMBOL_1)->descendentsCount() : 0;
  return numDescendants;
}

// compute the logarithm of the KT-estimator update multiplier
double CTNode::logKTMul(const symbol_t sym) {
  double denominator = log(symbolCount[SYMBOL_0] + symbolCount[SYMBOL_1] + 1);
  return log(symbolCount[sym] + 0.5) - denominator;
}

// Update the node after having observed a new symbol.
void CTNode::update(const symbol_t symbol, const int node_action) {
  if (node_action == NODE_UPDATE) {
    logProbKT += logKTMul(symbol);
    symbolCount[symbol] += 1;
  } else { //NODE_REVERT
    symbolCount[symbol] -= 1;
    logProbKT -= logKTMul(symbol);
  }
  double logProbW0 = 0.0;
  double logProbW1 = 0.0;
  if (child(SYMBOL_0)) {
    logProbW0 = child(SYMBOL_0)->getLogProbWeighted();
  }
  if (child(SYMBOL_1)) {
    logProbW1 = child(SYMBOL_1)->getLogProbWeighted();
  }
  double probW01_KTRatio = exp(logProbW0 + logProbW1 - logProbKT);
  if (probW01_KTRatio != probW01_KTRatio) {
    probW01_KTRatio = 0;
  }
  logProbWeighted = logProbKT + log1p(probW01_KTRatio) - log(2);
  probSanity();
}

void CTNode::updateLeaf(const symbol_t symbol, const int node_action) {
  //TODO: Code duplication, you lazy fuck!
  if (node_action == NODE_UPDATE) {
    logProbKT += logKTMul(symbol);
    symbolCount[symbol] += 1;
  } else {
    symbolCount[symbol] -= 1;
    logProbKT -= logKTMul(symbol);
  }
  logProbWeighted = logProbKT;
  probSanity();
}

void CTNode::probSanity() {
  assert(logProbKT <= 0);
  assert(logProbWeighted <= 0);
}

// Revert after MCTS look-ahead simulation is done.
void CTNode::revert(const symbol_t symbol) {
  // TODO : implement
}
