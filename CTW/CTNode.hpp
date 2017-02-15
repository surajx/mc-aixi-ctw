/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/

#ifndef __CTNODE_HPP__
#define __CTNODE_HPP__

#include "../common/types.hpp"
#include "../common/constants.hpp"

class CTNode {
 private:
  CTNode* parent;                      // Parent Node
  CTNode* children[2] = {NULL, NULL};  // List of childrens

  double logProbKT;        // log KT estimated probability
  double logProbWeighted;  // log weighted block probability

  // one slot for each symbol (SYMBOL_0, SYMBOL_1)
  uint_t symbolCount[2];  // a,b in CTW literature

 public:
  /**
  * Compute the logarithm of the KT-estimator update multiplier
  * TODO: More Documentation
  */
  double logKTMul(const symbol_t sym);

  /**
  * Calculate the logarithm of the weighted block probability.
  * be careful of numerical issues, use an identity for log(a+b)
  * so that you can work in logspace instead.
  * TODO: More Documentation
  */
  void updateLogProbability();

  /**
  * Update the node after having observed a new symbol.
  * TODO: More Documentation
  */
  void update(const symbol_t symbol, const int node_action);


  /**
  * Update just the leaf node with the observed symbol.
  * TODO: More Documentation
  */
  void updateLeaf(const symbol_t symbol, const int node_action);

  /*
  * Return the node to its state immediately prior to the last update.
  * TODO: More Documentation
  */
  void revert(const symbol_t symbol);

  /**
  * Log weighted blocked probability
  */
  double getLogProbWeighted() { return logProbWeighted; }

  /**
  * Log KT estimated probability
  */
  double getLogProbKT() { return logProbKT; }

  /**
  * Number of times this context has been visited
  */
  uint_t visits() { return symbolCount[SYMBOL_0] + symbolCount[SYMBOL_1]; }

  /**
  * Checkes sanity node variables.
  */
  void probSanity();

  /**
  * Child corresponding to a particular symbol
  */
  CTNode* child(symbol_t sym) { return children[sym]; }

  /**
  * Add child to parent
  */
  void addChild(symbol_t sym, CTNode* child, CTNode* parent);

  /**
  * Number of descendants of a node in the context tree
  */
  uint_t descendentsCount();

  CTNode();

  // Destructor
  ~CTNode();
};

#endif