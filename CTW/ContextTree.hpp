/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar, Baseline code Author
***************************************************************************************/

#ifndef __CONTEXTTREE_HPP__
#define __CONTEXTTREE_HPP__

#include <deque>

#include "../common/types.hpp"

#include "CTNode.hpp"

typedef std::deque<symbol_t> symbol_q_t;

class ContextTree {
 private:
  symbol_q_t sequenceHistory;  // Stores the entire history
  uint_t maxTreeDepth;         // the maximum depth of the context tree
  CTNode* rootNode;            // rootNode of the context trees

  /**
  * TODO: Consider renaming because it does both update and revert.
  * Well maybe its okay, revert is kind of an update, think about it!
  *
  * Recursively updates/reverts the prediction suffix tree with the
  * latest symbol
  *
  * @param sym The binary symbol.
  * @param depth The current depth of the context tree during recursion.
  * @param node The current node that is being updated/reverted.
  * @param node_action Flag that idicated if its a revert or update.
  */
  void update(const symbol_t sym,
              uint_t depth,
              CTNode* node,
              const int node_action);

  /**
  * Delete the tree whose root node is given.
  *
  * @param node The root node of the tree that needs to be deleted.
  */
  void deleteTree(CTNode* node);

 public:
  /**
  * Constructor to instantiate a Context Tree of specified depth
  *
  * @param depth The depth of the context tree.
  */
  ContextTree(uint_t depth);

  /**
  * Resets the context tree statistics
  */
  void clear();

  /**
  * Update the context tree with a new binary symbol
  *
  * @param sym The binary symbol.
  */
  void update(const symbol_t sym);

  /**
  * Update the context tree with a list of binary symbols.
  *
  * @param symbol_list vector of binary symbols.
  */
  void update(const symbol_list_t& symbol_list);

  /**
  * Updates the history statistics, without touching the context tree
  * use it to update the history of those percept bits that predict an action
  * bit.
  *
  * @param symbol_list vector of binary symbols.
  */
  void updateHistory(const symbol_list_t& symbol_list);

  /**
  * Removes the most recently observed symbol from the context tree.
  */
  void revert();

  /**
  * Shrinks the history down to a given size.
  *
  * @param new_size The new (reduced) size of the history.
  */
  void revertHistory(uint_t newsize);

  /**
  * Shrinks the history down to by one bit.
  */
  void revertHistory();

  /**
  * Estimates the probability of observing a particular symbol
  *
  * @param  sym The given binary symbol
  *
  * @return double The probability of observing the given symbol.
  */
  double predict(symbol_t sym);

  /**
  * Estimates the probability of observing a particular sequence of symbols.
  *
  * @param  symbol_list The given binary symbol sequence
  *
  * @return double The probability of observing the given symbol sequence.
  */
  double predict(symbol_list_t& symbol_list);

  /**
  * Predict what the next symbol would be according to the context tree
  * statistics.
  *
  * @return symbol_t The next symbol according to the Context Tree statistics.
  */
  symbol_t predictSymbol();

  /**
  * Generate a specified number of random symbols distributed according to the
  * context tree statistics.
  *
  * @param predictedSymbols Reference to empty symbol list which stores the
  *                         predicted symbols.
  * @param num The number of symbols to be generated.
  */
  void genNextSymbols(symbol_list_t& predictedSymbols, uint_t num);

  /**
  * Generate a specified number of random symbols distributed according to the
  * context tree statistics AND update the context tree with the newly generated
  * bits.
  *
  * @param predictedSymbols Reference to empty symbol list which stores the
  *                         predicted symbols.
  * @param num The number of symbols to be generated.
  */
  void genNextSymbolsAndUpdate(symbol_list_t& predictedSymbols, uint_t num);

  /**
  * The logarithm of the block probability of the whole sequence
  *
  * @return double The block probability of the sequence seen so far.
  */
  double getLogBlockProbability();

  /**
  * Get the n'th history symbol, NULL if doesn't exist
  *
  * @param n The position in the observed sequence.
  *
  * @return symbol_t* n'th history symbol.
  */
  symbol_t* nthHistorySymbol(uint_t n);

  /**
  * The maximum depth of the context tree.
  *
  * @return uint_t Context Tree depth.
  */
  uint_t depth() { return maxTreeDepth; }

  /**
  * The size of the stored history.
  *
  * @return uint_t History Size.
  */
  uint_t historySize() { return sequenceHistory.size(); }

  /**
  * Number of nodes in the context tree.
  *
  * @return uint_t Context Tree node count.
  */
  uint_t size() { return rootNode ? rootNode->descendentsCount() : 0; }

  /**
  * Destructor
  */
  ~ContextTree();
};

#endif
