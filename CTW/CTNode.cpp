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

// Update the KT-estimator and the symbol count based on node_action.
void CTNode::updateLogKT(const symbol_t symbol, const int node_action) {
  if (node_action == NODE_UPDATE) {
    logProbKT += logKTMul(symbol);
    symbolCount[symbol] += 1;
  } else {  // NODE_REVERT
    symbolCount[symbol] -= 1;
    logProbKT -= logKTMul(symbol);
  }
}

// Update the node after having observed a new symbol.
void CTNode::update(const symbol_t symbol, const int node_action) {
  // Update KT-estimator and symbol count.
  updateLogKT(symbol, node_action);
  double logProbW0 = 0.0;
  double logProbW1 = 0.0;

  // If child exists get the weighted log(prob) else use default value of 0.0
  if (child(SYMBOL_0)) {
    logProbW0 = child(SYMBOL_0)->getLogProbWeighted();
  }
  if (child(SYMBOL_1)) {
    logProbW1 = child(SYMBOL_1)->getLogProbWeighted();
  }

  // log((p_kt + p_w0.p_w1)/2) = log(p_kt) + log(1+(p_w0.p_w1)/(p_kt)) - log(2)

  // (p_w0.p_w1)/(p_kt) = exp(log(p_w0) + log(p_w1) - log(p_kt))
  double probW01_KTRatio = exp(logProbW0 + logProbW1 - logProbKT);

  // Detecting underflow NaN!=NaN
  // if (p_w0.p_w1)/(p_kt)==NaN then log(1 + (p_w0.p_w1)/(p_kt))=log(1)=0
  if (probW01_KTRatio != probW01_KTRatio) {
    probW01_KTRatio = 0;
  }

  // log(p_w) = log(p_kt) + log(1 + (p_w0.p_w1)/(p_kt)) - log(2)
  logProbWeighted = logProbKT + log1p(probW01_KTRatio) - log(2);
  probSanity();
}

// If node is a leaf then log(p_kt) is same as log(p_w)
void CTNode::updateLeaf(const symbol_t symbol, const int node_action) {
  updateLogKT(symbol, node_action);
  logProbWeighted = logProbKT;
  probSanity();
}

void CTNode::probSanity() {
  assert(logProbKT <= 0);
  assert(logProbWeighted <= 0);
}
