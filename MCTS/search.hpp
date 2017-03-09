#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include "../AIXI/agent.hpp"

#include "../common/types.hpp"

typedef unsigned long long visits_t;

class SearchNode {
 private:
  bool m_chance_node;  // true if this node is a chance node, false if action
  unsigned int m_children;  // chance (action) nodes have |X| (|A|) children
  double m_mean;            // the expected reward of this node
  visits_t m_visits;        // number of times the search node has been visited
  std::vector<SearchNode*> children;

  unsigned int numActions;          // number of actions
  unsigned int numPercepts;         // number of percepts
  unsigned int numObservationBits;  // number of observation bits
  unsigned int numRewardBits;       // number of reward bits
  double C;                         // exploration-exploitation constant C
  unsigned int m;                   // also known as horizon, or max tree depth.

  Agent* agent;

 public:
  /**
  * TODO: Documentation
  * Constructor
  */
  SearchNode(Agent* ai, bool is_chance_node, unsigned int num_children);

  /**
  * Returns best action.
  * If more than one, return an uniformly random best action.
  */
  action_t bestAction() const;

  /**
  * Select action based on UCT
  */
  SearchNode* selectAction(unsigned int horiz);

  /**
  * Performs a sample run through this node and its children returning the
  * accumulated reward from this sample run.
  */
  reward_t sample(unsigned int horiz);

  // number of times the search node has been visited
  visits_t visits(void) const { return m_visits; }

  // determine the expected reward from this node
  reward_t expectation(void) const { return m_mean; }

  // vector with children
  std::vector<SearchNode*> getChildren(void) const { return children; }

  /**
  * Recursively destruct the SearchNode and its children.
  * Do not destruct the newly assigned root node.
  */
  ~SearchNode(void);
};

class SearchTree {
 private:
  SearchNode* rootNode;
  unsigned int numActions;          // number of actions
  unsigned int numPercepts;         // number of percepts
  unsigned int numObservationBits;  // number of observation bits
  unsigned int numRewardBits;       // number of reward bits
  double C;                         // exploration-exploitation constant C
  unsigned int m;                   // also known as horizon, or max tree depth.
  Agent* agent;
  bool isFirst = true;

 public:
  SearchTree(Agent* agent);
  action_t search(percept_t prev_obs, percept_t prev_rew, action_t prev_act);
  reward_t playout(unsigned int playout_len);
  void pruneTree(percept_t prev_obs, percept_t prev_rew, action_t prev_act);
  SearchNode* getRootNode() { return rootNode; }
  ~SearchTree(void);
};

#endif  // __SEARCH_HPP__
