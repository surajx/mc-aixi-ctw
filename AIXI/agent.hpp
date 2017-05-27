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
  * Encodes an action as a list of symbols.
  *
  * @param symbol_list_t& symlist A list of symbols into which the action is
  *        encoded
  * @param action_t action The action to be encoded
  */
  void encodeAction(symbol_list_t& symlist, action_t action) const;

  /**
  * Encodes a percept (observation, reward) as a list of symbols.
  *
  * @param symbol_list_t& symlist A list of symbols into which the observation
  *        and reward are to be encoded
  * @param percept_t observation The observation to be encoded
  * @param percept_t reward The reward to be encoded
  */
  void encodePercept(symbol_list_t& symlist,
                     percept_t observation,
                     percept_t reward) const;

  /**
  * Decodes the observation from a list of symbols.
  *
  * @param symbol_list_t& symlist A list of symbols representing an action
  *
  * @return action_t The decoded action
  */
  action_t decodeAction(const symbol_list_t& symlist) const;

  /**
  * Decodes the reward from a list of symbols.
  *
  * @param symbol_list_t& symlist A list of symbols representing a reward
  *
  * @return percept_t The decoded reward
  */
  percept_t decodeReward(const symbol_list_t& symlist) const;

 public:
  /**
  * Constructor to initialize an MC-AIXI-CTW agent from the command line
  * arguments.
  *
  * @param options_t& options Command line arguments
  */
  Agent(options_t& options);

  /**
  * Get the current lifetime of the agent in cycles.
  *
  * @return lifetime_t The liftime of the agent
  */
  lifetime_t lifetime(void) const;

  /**
  * Initialize the planner by creating a SearchTree
  */
  bool initPlanner();

  /**
  * Increments the agent's age by 1.
  */
  void incAgentAge();

  /**
  * TODO
  *
  * @param
  *
  * @return ContextTree* The current
  */
  ContextTree* getModel();

  /**
  * Call on the context tree to print its current model context.
  */
  void printCurentModelContext(void) const;

  /**
  * Choose a next action by calling the search function in search.cpp.
  *
  * @param percept_t prev_obs The previous observation that the environment
  *        generated
  * @param percept_t prev_rew The previous reward that the environment
  *        generated
  * @param percept_t prev_act The previous action that the agent
  *        generated
  *
  * @return action_t The action deemed best by the planner
  */
  action_t getPlannedAction(percept_t prev_obs,
                            percept_t prev_rew,
                            action_t prev_act);

  /**
  * Get the planner that the agent uses to choose its next action
  *
  * @return SearchTree* Pointer to a SearchTree
  */
  SearchTree* getPlanner();

  /**
  * Get the root node from the planner
  *
  * @return SearchNode* The root node
  */
  SearchNode* getPlannerRootNode();

  /**
  * Get the total accumulated reward across an agents lifespan.
  *
  * @return reward_t Total accumulated reward
  */
  reward_t reward(void) const;

  /**
  * Get the average reward received by the agent at each time step,
  *
  * @return reward_t The average reward received at each time step
  */
  reward_t averageReward(void) const;

  /**
  * Get the maximum possible reward in a single time instant. This value is
  * determined by the configuration of the environment.
  *
  * @return reward_t the maximum reward
  */
  reward_t maxReward(void) const;

  /**
  * Get the minimum possible reward in a single time instant. This value is
  * determined by the configuration of the environment.
  *
  * @return reward_t the minimum reward
  */
  reward_t minReward(void) const;

  /**
  * Get the number of obseration bits
  *
  * @return uint_t the number of observation bits
  */
  uint_t getNumObservationBits(void) const;

  /**
  * Get the number of reward bits
  *
  * @return uint_t the number of reward bits
  */
  uint_t getNumRewardBits(void) const;

  /**
  * Get the number of action bits, i.e. the number of bits to encode an action
  *
  * @return uint_t The number of bits to encode an action
  */
  uint_t getNumActionBits(void) const;

  /**
  * Get the number of distinct actions, i.e. the size of the action set
  *
  * @return uint_t The number of distinct actions
  */
  uint_t getNumActions(void) const;

  /**
  * Get the number of simulations per planning cycle
  *
  * @return uint_t the number of simulations
  */
  uint_t numSimulations(void) const;

  /**
  * Get the C parameter of the UCB algorithm
  *
  * @return uint_t the C parameter
  */
  uint_t getExploreExploitRatio(void) const;

  /**
  * Get the length of the stored history for an agent
  *
  * @return size_t The length of the stored history
  */
  size_t historySize(void) const;

  /**
  * Get the length of the search horizon used by the agent
  *
  * @return size_t Length of search horizon
  */
  size_t horizon(void) const;

  /**
  * Generate an action uniformly at random
  *
  * @return action_t A randomly generated action
  */
  action_t genRandomAction(void) const;

  /**
  * Generate a percept distributed according to our history statistics
  * without updating our mixture environment model with it.
  *
  * @param uint_t percept_size the number of bits in the percept.
  *
  * @return percept_t the generated percept
  */
  percept_t genPercept(uint_t percept_size) const;

  /**
  * Generate a percept distributed to our history statistics, and update our
  * mixture environment model with it
  *
  * @param uint_t percept_size the number of bits in the percept.
  *
  * @return percept_t the generated percept
  */
  percept_t genPerceptAndUpdate(uint_t percept_size);

  /**
  * Update agent's internal  model of the world due to receiving a percept.
  *
  * @param percept_t observation The observation that the environment generated
  * @param percept_t reward The reward that the environment generated
  */
  void modelUpdate(percept_t observation, percept_t reward);

  /**
  * Update agent's internal model of the world due to performing an action.
  *
  * @param action_t action The action that was performed
  */
  void modelUpdate(action_t action);

  /**
  * Revert agent's internal model of the world to that of a previous time
  * cycle.
  *
  * @param uint_t newSearchHorizon The new search horizon
  */
  bool modelRevert(uint_t newSearchHorizon);

  /**
  * Resets the agent
  */
  void reset(void);

  /**
  * Destroy the agent and the corresponding context tree
  */
  ~Agent(void);
};

#endif  // __AGENT_HPP__
