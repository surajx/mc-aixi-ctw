#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include "../AIXI/agent.hpp"

#include "../common/types.hpp"

class SearchNode;

// MCTS constants
static unsigned int numActions; // number of actions
static unsigned int obsBits; // number of observation bits
static unsigned int rewBits; // number of reward bits
static double C; // exploration-exploitation constant C
static unsigned int m; // also known as horizon, or max tree depth.
SearchNode *root_ptr;

// determine the best action by searching ahead
extern action_t search(Agent &agent);

#endif // __SEARCH_HPP__
