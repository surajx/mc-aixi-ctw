#include "search.hpp"
#include "../AIXI/agent.hpp"

#include <iostream>

typedef unsigned long long visits_t;


// search options
static const visits_t     MinVisitsBeforeExpansion = 1;
static const unsigned int MaxDistanceFromRoot  = 100;
static size_t             MaxSearchNodes;

// contains information about a single "state"
class SearchNode {

public:

	SearchNode(bool is_chance_node);

	// determine the next action to play
	action_t selectAction(Agent &agent) const; // TODO: implement

	// determine the expected reward from this node
	reward_t expectation(void) const { return m_mean; }

	// perform a sample run through this node and it's children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int dfr); // TODO: implement

	// number of times the search node has been visited
	visits_t visits(void) const { return m_visits; }

private:

	bool m_chance_node; // true if this node is a chance node, false otherwise
	unsigned int m_children; // chance (action) nodes have |X| (|A|) children
	double m_mean;      // the expected reward of this node
	visits_t m_visits;  // number of times the search node has been visited


	// TODO: decide how to reference child nodes
	//  e.g. a fixed-size array
};

// simulate a path through a hypothetical future for the agent within it's
// internal model of the world, returning the accumulated reward.
static reward_t playout(Agent &agent, unsigned int playout_len) {
	return 0; // TODO: implement
}

// determine the best action by searching ahead using MCTS
extern action_t search(Agent &agent) {
	unsigned int num_simulations = agent.numSimulations();
	unsigned int num_actions = agent.numActions();
	double C = agent.exploreExploitRatio();
	size_t m = agent.horizon();
	unsigned int num_obs = agent.numObservations();
	unsigned int num_rew = agent.numRewards();
	SearchNode root(false); // create root (decision) node
	// manual initialize?
	// for i in range(num_simulations)
	// 	prune
	// 	sample(root, m)
	// return bestAction(root)
	return agent.genRandomAction(); // TODO: implement
	//std::cout << "num_obs & num_rew: " << num_obs << " " << num_rew << std::endl;
}
