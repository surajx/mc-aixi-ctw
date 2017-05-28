/******************************************************************************
** CTNode Class represents a node in the ContextTree for the CTW method.
** Detailed Documentation of each functionality is given in the header file.
**
** Author: Suraj Narayanan Sasikumar
*******************************************************************************/

#include <cmath>
#include <cassert>
#include <iostream>

#include "../common/types.hpp"
#include "../common/constants.hpp"

#include "CTNode.hpp"

CTNode::CTNode() {
  logProbKT = 0.0;
  logProbWeighted = 0.0;
  symbolCount[SYMBOL_0] = 0;
  symbolCount[SYMBOL_1] = 0;

  node_count += 1;
}

uint_t CTNode::node_count = 0;

CTNode::~CTNode() {
  if (children[SYMBOL_0] != NULL) {
    delete children[SYMBOL_0];
    children[SYMBOL_0] = NULL;
  }
  if (children[SYMBOL_1] != NULL) {
    delete children[SYMBOL_1];
    children[SYMBOL_1] = NULL;
  }

  node_count -= 1;
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
    if (symbolCount[symbol] == 0) {
      std::cout << "WARNING: Revert Called on a CTW Node which has not seen "
                   "any symbol."
                << std::endl;
      std::cout << "DEBUG: Revert cancelled (nothing to revert)." << std::endl;
      std::cout << "DEBUG: Node: " << this << std::endl;
      std::cout << "DEBUG: symbolCount[symbol]=" << symbolCount[symbol]
                << std::endl;
      std::cout << "DEBUG: logProbKT=" << logProbKT << std::endl;
      return;
    }
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
  if (probW01_KTRatio > 1) {
    probW01_KTRatio = exp(logProbKT - logProbW0 - logProbW1);

    // log(p_w) = log(p_w0) + log(p_w1) + log(1 + (p_kt)/(p_w0.p_w1)) - log(2)
    logProbWeighted = logProbW0 + logProbW1;
  } else {
    // log(p_w) = log(p_kt) + log(1 + (p_w0.p_w1)/(p_kt)) - log(2)
    logProbWeighted = logProbKT;
  }
  // Detecting underflow NaN!=NaN
  // if ratio underflows then log(1 + ratio)=log(1)=0
  if (probW01_KTRatio != probW01_KTRatio) {
    probW01_KTRatio = 0;
  }
  logProbWeighted += log1p(probW01_KTRatio) - log(2);
  probSanity();
}

// If node is a leaf then log(p_kt) is same as log(p_w)
void CTNode::updateLeaf(const symbol_t symbol, const int node_action) {
  updateLogKT(symbol, node_action);
  logProbWeighted = logProbKT;
  probSanity();
}

void CTNode::probSanity() {
  assert(
      "Log Probability should be always less that 0, with a proposed tolerance "
      "of 1.0e-10. " &&
      logProbKT <= 1.0e-10);
  assert(
      "Log Probability should be always less that 0, with a proposed tolerance "
      "of 1.0e-10. " &&
      logProbWeighted <= 1.0e-10);
  if (logProbKT > 0)
    logProbKT = 0;
  if (logProbWeighted > 0)
    logProbWeighted = 0;
}
