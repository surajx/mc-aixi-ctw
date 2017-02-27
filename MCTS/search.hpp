#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include "../AIXI/agent.hpp"

#include "../common/types.hpp"

class SearchNode;

// MCTS constants
static unsigned int numActions; // number of actions
static unsigned int numPercepts; // number of percepts
static unsigned int obsBits; // number of observation bits
static unsigned int rewBits; // number of reward bits
static double C; // exploration-exploitation constant C
static unsigned int m; // also known as horizon, or max tree depth.
static SearchNode*  root_ptr; // pointer to the root node
static unsigned int test_tree_size; // test

// map an (o,r) pair to a unique index
percept_t perceptIndex(percept_t obs, percept_t rew);

// determine the best action by searching ahead
extern action_t search(Agent &agent, percept_t prev_obs, percept_t prev_rew, action_t prev_act, bool tree_initialized);

static reward_t playout(Agent &agent, unsigned int playout_len);

#endif // __SEARCH_HPP__
