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
  * Constructor for Context Tree Node.
  */
  CTNode();

  //Node count
  static uint_t node_count;

  /**
  * Add child to parent
  * TODO: More Documentation
  */
  void addChild(symbol_t sym, CTNode* child, CTNode* parent);

  /**
  * Number of descendants of a node in the context tree
  * TODO: More Documentation
  */
  uint_t descendentsCount();  

  /**
  * Compute the logarithm of the KT-estimator update multiplier
  * TODO: More Documentation
  */
  double logKTMul(const symbol_t sym);

  /**
  * Update the logarithm of the KT-estimator.
  * TODO: More Documentation
  */
  void updateLogKT(const symbol_t symbol, const int node_action);

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

  /**
  * Checkes sanity node variables.
  */
  void probSanity();

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
  * Child corresponding to a particular symbol
  */
  CTNode* child(symbol_t sym) { return children[sym]; }

  // The Destructor
  ~CTNode();
};

#endif