/****************************************************************************************
** Implements the classes SearchNode and SearchTree. This module implements
** a variant of the UCT algorithm for Monte Carlo tree search with adaptations
** to deal with planning and predicting in unkown environments.
**
** Author: Arie Slobbe
***************************************************************************************/

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
  * Constructor to initialize a SearchNode for a given agent. SearchNodes
  * are the basic elements from which the game tree is constructed.
  *
  * @param Agent* ai The agent using the SearchNode for its planning
  * @param bool is_chance_node Specifies whether the searchnode is an action node
  *        or a chance node.
  * @param unsigned int num_children The number of child nodes.
  */
  SearchNode(Agent* ai, bool is_chance_node, unsigned int num_children);

  //Node count
  static uint_t node_count;


  /**
  * Returns best action.
  * If more than one, return uniformly at random one of the best actions.
  *
  * @return action (One of) the action(s) with the highest expected reward.
  */
  action_t bestAction() const;

  /**
  * Select action based on the UCT algorithm for Monte Carlo Tree Search.
  *
  * @param unsigned int horiz The remaining search depth w.r.t the current node.
  *
  * @return SearchNode The best action (child node) according to the UCT heuristic.
  */
  SearchNode* selectAction(unsigned int horiz);

  /**
  * Performs a sample run through this node and its children returning the
  * accumulated reward from this sample run.
  *
  * @param unsigned int horiz The remaining search depth w.r.t the current node.
  *
  * @return reward The accumulated reward of the path explored in the game tree.
  */
  reward_t sample(unsigned int horiz);


  /**
  * Number of times the search node has been visited.
  *
  * @return visits The number of visits.
  */
  visits_t visits(void) const { return m_visits; }

  /**
  * Return the expected reward from this node. Expected reward is estimated
  * from sampled paths in the game tree that involved this node.
  *
  * @return reward The (mean) expected reward from this node.
  */
  reward_t expectation(void) const { return m_mean; }

  /**
  * Select action based on the UCT algorithm for Monte Carlo Tree Search
  *
  * @return vector<SearchNode*> Vector with pointers to child nodes (and NULL
  *         pointers to noninitialized child nodes).
  */
  std::vector<SearchNode*> getChildren(void) const { return children; }

  /**
  * Recursively destruct the SearchNode and its children.
  * Does not destruct the newly assigned root node.
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
