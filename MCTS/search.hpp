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

#include <unordered_map>

typedef unsigned long long visits_t;

class SearchNode {
 private:
  bool m_chance_node;  // true if this node is a chance node, false if action
  unsigned int m_children;  // chance (action) nodes have |X| (|A|) children
  double m_mean;            // the expected reward of this node
  visits_t m_visits;        // number of times the search node has been visited
  std::unordered_map<action_t, SearchNode*> children;

  unsigned int numActions;          // number of actions
  unsigned int numPercepts;         // number of percepts
  unsigned int numObservationBits;  // number of observation bits
  unsigned int numRewardBits;       // number of reward bits
  double C;                         // exploration-exploitation constant C
  unsigned int m;                   // also known as horizon, or max tree depth.

 public:
  /**
  * Constructor to initialize a SearchNode for a given SearchTree. SearchNodes
  * are the basic elements from which the search tree is constructed.
  *
  * @param bool is_chance_node Specifies whether the searchnode is an action node
  *        or a chance node.
  */
  SearchNode(bool is_chance_node);

  // Node count
  static uint_t node_count;

  // Set the agent instance as static
  static Agent* agent;


  /**
  * Returns best action.
  * If more than one, return uniformly at random one of the best actions.
  *
  * @return action (One of) the action(s) with the highest expected reward.
  */
  action_t bestAction();

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
  * @return unordered_map<action_t, SearchNode*> Unordered map with actions as
  *         keys and pointers to child nodes as values.
  */
  std::unordered_map<action_t, SearchNode*> getChildren(void) const { return children; }

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
  bool isFirst = true;              // Do not prune during very fist search

 public:
   /**
   * Constructor to initialize a SearchTree for a given agent. A SearchTree
   * comprises the planning module of the MC-AIXI-CTW agent.
   *
   * @param Agent* agent The agent using the SearchTree for its planning
   */
  SearchTree(Agent* agent);

  /**
  * The main function of the search file. The agent calls search to choose its
  * next action.
  *
  * @param percept_t prev_obs The previous observation that the environment
  *        generated
  * @param percept_t prev_rew The previous reward that the environment
  *        generated
  * @param percept_t prev_act The previous action that the agent
  *        generated
  *
  * @return action_t action The child node of the root with the highest expected
  *         reward
  */
  action_t search(percept_t prev_obs, percept_t prev_rew, action_t prev_act);

  /**
  * Executes a playout starting from a search node in order to estimate the
  * value of itself and its parent nodes. A random playout policy is applied and
  * reward is accumulated to pass back to the search nodes.
  *
  * @param unsigned int playout_len The remaining search depth
  *
  * @return reward_t The total reward accumulated in the playout
  */
  reward_t playout(unsigned int playout_len);

  /**
  * Retain useful computations from the previous agent-environment interaction
  * cycle while reclaiming memory allocated to nodes that are not necessary
  * anymore.
  *
  * @param percept_t prev_obs The previous observation that the environment
  *        generated
  * @param percept_t prev_rew The previous reward that the environment
  *        generated
  * @param percept_t prev_act The previous action that the agent
  *        generated
  */
  void pruneTree(percept_t prev_obs, percept_t prev_rew, action_t prev_act);

  /**
  * Get the SearchNode which is assigned as the root node
  *
  * @return SearchNode* The root node of the tree
  */
  SearchNode* getRootNode() { return rootNode; }

  /**
  * Desctructor of the SearchTree
  */
  ~SearchTree(void);
};

#endif  // __SEARCH_HPP__
