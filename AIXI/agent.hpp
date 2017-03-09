#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <iostream>

#include "../common/types.hpp"

class SearchTree;
class SearchNode;
class ContextTree;

class Agent {
 private:
  uint_t numSimPerCycle;     // number of simulations per planning cycle (Arie)
  uint_t numActions;         // number of actions
  uint_t numActionBits;      // number of bits to represent an action
  uint_t numObervationBits;  // number of bits to represent an observation
  uint_t numRewardBits;      // number of bits to represent a reward
  size_t searchHorizon;      // length of the search horizon

  double exploreExploitRatio;  // parameter C in the UCB algorithm
  reward_t totalReward;        // Total reward received by the agent

  // Context Tree representing the agent's beliefs
  ContextTree* contextTree;
  SearchTree* planner;

  // How many time cycles the agent has been alive
  lifetime_t agentAge;

  // True if the last update was a percept update
  bool isLastUpdateAPercept;

  /**
  * TODO: More Documentation
  * Action sanity check
  */
  bool isActionOk(action_t action) const;

  /**
  * TODO: More Documentation
  * Reward Sanity Check
  */
  bool isRewardOk(reward_t reward) const;

  /**
  * Encodes an action as a list of symbols
  */
  void encodeAction(symbol_list_t& symlist, action_t action) const;

  /**
  * Encodes a percept (observation, reward) as a list of symbols
  */
  void encodePercept(symbol_list_t& symlist,
                     percept_t observation,
                     percept_t reward) const;

  /**
  * Decodes the observation from a list of symbols
  */
  action_t decodeAction(const symbol_list_t& symlist) const;

  /**
  * Decodes the reward from a list of symbols
  */
  percept_t decodeReward(const symbol_list_t& symlist) const;

 public:
  // construct a learning agent from the command line arguments
  Agent(options_t& options);

  // current lifetime of the agent in cycles
  lifetime_t lifetime(void) const;

  // Init Planner
  bool initPlanner();

  // Get planned action
  action_t getPlannedAction(percept_t prev_obs,
                            percept_t prev_rew,
                            action_t prev_act);

  // Get planner
  SearchTree* getPlanner();

  // Get planner root Node
  SearchNode* getPlannerRootNode();

  // the total accumulated reward across an agents lifespan
  reward_t reward(void) const;

  // the average reward received by the agent at each time step
  reward_t averageReward(void) const;

  // maximum reward in a single time instant
  reward_t maxReward(void) const;

  // minimum reward in a single time instant
  reward_t minReward(void) const;

  // getter for number of observation bits
  uint_t getNumObservationBits(void) const;

  // getter for number of reward bits
  uint_t getNumRewardBits(void) const;

  // getter for number of action bits
  uint_t getNumActionBits(void) const;

  // number of distinct actions
  uint_t getNumActions(void) const;

  // number of simulations per planning cycle
  uint_t numSimulations(void) const;

  // The C parameter of the UCB algorithm
  uint_t getExploreExploitRatio(void) const;

  // the length of the stored history for an agent
  size_t historySize(void) const;

  // length of the search horizon used by the agent
  size_t horizon(void) const;

  // generate an action uniformly at random
  action_t genRandomAction(void) const;

  /**
  * Generate a percept distributed according to our history statistics.
  */
  percept_t genPercept(uint_t percept_size) const;

  /**
  * Generate a percept distributed to our history statistics, and update our
  * mixture environment model with it
  */
  percept_t genPerceptAndUpdate(uint_t percept_size);

  // Update agent's internal  model of the world due to receiving a percept.
  void modelUpdate(percept_t observation, percept_t reward);

  // Update agent's internal model of the world due to performing an action.
  void modelUpdate(action_t action);

  // Revert agent's internal model of the world to that of a previous time
  // cycle.
  bool modelRevert(uint_t newSearchHorizon);

  // Resets the agent
  void reset(void);

  // Destroy the agent and the corresponding context tree
  ~Agent(void);
};

#endif  // __AGENT_HPP__
