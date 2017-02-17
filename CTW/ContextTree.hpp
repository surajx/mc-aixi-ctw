/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/

#ifndef __CONTEXTTREE_HPP__
#define __CONTEXTTREE_HPP__

#include <deque>

#include "../common/constants.hpp"
#include "../common/types.hpp"

typedef std::deque<symbol_t> symbol_q_t;

class CTNode {
 private:
  CTNode* parent;       // Parent Node
  CTNode* children[2] = {NULL, NULL};  // List of childrens

  double logProbEstimated;  // log KT estimated probability
  double logProbWeighted;   // log weighted block probability

  // one slot for each symbol (SYMBOL_0, SYMBOL_1)
  uint_t symbolCount[2];  // a,b in CTW literature

  /**
  * Compute the logarithm of the KT-estimator update multiplier
  * TODO: implement and more documentation
  */
  double logKTMul(symbol_t sym);

  /**
  * Calculate the logarithm of the weighted block probability.
  * be careful of numerical issues, use an identity for log(a+b)
  * so that you can work in logspace instead.
  * TODO: implement and more documentation
  */
  void updateLogProbability();

  /**
  * Update the node after having observed a new symbol.
  * TODO: implement and more documentation
  */
  void update(const symbol_t symbol);

  /*
  * Return the node to its state immediately prior to the last update.
  * TODO: implement in predict.cpp
  */
  void revert(const symbol_t symbol);

 public:
  /**
  * Log weighted blocked probability
  */
  double getLogProbWeighted() { return logProbWeighted; }

  /**
  * Log KT estimated probability
  */
  double getLogProbEstimated() { return logProbEstimated; }

  /**
  * Number of times this context has been visited
  */
  uint_t visits() { return symbolCount[SYMBOL_0] + symbolCount[SYMBOL_1]; }

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

class ContextTree{
 private:
  symbol_q_t sequenceHistory;
  uint_t maxTreeDepth;  // the maximum depth of the context tree
  CTNode* rootNode;

 public:
  // create a context tree of specified maximum depth
  ContextTree(uint_t depth);

  // clear the entire context tree
  void clear();

  // updates the context tree with a new binary symbol
  void update(const symbol_t sym);  // TODO: implement in predict.cpp
  // update the context tree with a list of symbols.
  void update(const symbol_list_t& symbol_list);
  // add a symbol to the history without updating the context tree.
  void updateHistory(const symbol_list_t& symbol_list);

  // Recalculate the log weighted probability for this node.
  void updateLogProbability();

  // removes the most recently observed symbol from the context tree
  void revert();

  // shrinks the history down to a former size
  void revertHistory(uint_t newsize);

  // the estimated probability of observing a particular symbol or sequence
  double predict(symbol_t sym);               // TODO: implement in predict.cpp
  double predict(symbol_list_t symbol_list);  // TODO: implement in predict.cpp

  // generate a specified number of random symbols
  // distributed according to the context tree statistics
  symbol_t predictSymbol();

  // generate a specified number of random symbols distributed according to
  // the context tree statistics and update the context tree with the newly
  // generated bits
  void predictMutipleSymbols(uint_t numSymToPredict,
                             symbol_list_t& predictedSymbols);

  // the logarithm of the block probability of the whole sequence
  double getLogBlockProbability();

  // get the n'th history symbol, NULL if doesn't exist
  symbol_t* nthHistorySymbol(uint_t n);

  // the depth of the context tree
  uint_t depth() { return maxTreeDepth; }

  // the size of the stored history
  uint_t historySize() { return sequenceHistory.size(); }

  // number of nodes in the context tree
  uint_t size() { return rootNode ? rootNode->descendentsCount() : 0; }

  // Destructor
  ~ContextTree();
};

#endif
