/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/

#ifndef __CONTEXTTREE_HPP__
#define __CONTEXTTREE_HPP__

#include <deque>

#include "../common/types.hpp"

#include "CTNode.hpp"

typedef std::deque<symbol_t> symbol_q_t;

class ContextTree {
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
  void update(const symbol_t sym);

  // update the context tree with a list of symbols.
  void update(const symbol_list_t& symbol_list);

  // recursive branch update
  void update(const symbol_t sym, uint_t depth, CTNode* node);

  // add a symbol to the history without updating the context tree.
  void updateHistory(const symbol_list_t& symbol_list);

  // removes the most recently observed symbol from the context tree
  void revert();

  // shrinks the history down to a former size
  void revertHistory(uint_t newsize);

  // the estimated probability of observing a particular symbol or sequence
  double predict(symbol_t sym);

  // TODO: Documentaion
  double predict(symbol_list_t symbol_list);

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

  // Debug
  CTNode* getRootNode() { return rootNode; };

  // Destructor
  ~ContextTree();
};

#endif
