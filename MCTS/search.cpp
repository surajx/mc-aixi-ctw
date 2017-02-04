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

	SearchNode(bool is_chance_node, unsigned int num_children){
			m_visits = 0;
			m_mean = 0;
			m_children = num_children;
			children.resize(m_children);
			std::cout << "creating search node" << std::endl;
	}

	// determine the next action to play
	action_t selectAction(Agent &agent) const; // TODO: implement

	// determine the expected reward from this node
	reward_t expectation(void) const { return m_mean; }

	// perform a sample run through this node and it's children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int dfr) {
		std::cout << "entering sample function call " << std::endl;
		SearchNode child(false, 2);
		children[2] = &child;
		std::cout << "children[2]: " << children[2] << std::endl;
		return 0;
	}

	// number of times the search node has been visited
	visits_t visits(void) const { return m_visits; }

private:

	bool m_chance_node; // true if this node is a chance node, false otherwise
	unsigned int m_children; // chance (action) nodes have |X| (|A|) children
	double m_mean;      // the expected reward of this node
	visits_t m_visits;  // number of times the search node has been visited
	std::vector<SearchNode*> children;
};

// simulate a path through a hypothetical future for the agent within it's
// internal model of the world, returning the accumulated reward.
static reward_t playout(Agent &agent, unsigned int playout_len) {
	return 0; // TODO: implement
}

// initialize the tree by initializing a root node or pruning an existing tree
void initializeTree(Agent &agent) {
	std::cout << "initializing" << std::endl;
	// initialize values
	numActions = agent.numActions();
	C = agent.exploreExploitRatio();
	m = agent.horizon();
	obsBits = agent.numObservations();
	rewBits = agent.numRewards();
	root_ptr = new SearchNode(false,numActions);
}

void pruneTree(Agent &agent) {
	std::cout << "pruning" << std::endl;
	SearchNode* root_ptr = new SearchNode(false,numActions);
}

// determine the best action by searching ahead using MCTS
extern action_t search(Agent &agent) {
	std::cout << "lifetime: " << agent.lifetime() << std::endl;
	if (agent.lifetime() == 0) {
		initializeTree(agent);
	} else {
		pruneTree(agent);
	}
	SearchNode root = *root_ptr;
	std::cout << "root.visits(): " << root.visits() << std::endl;
	for (unsigned int sim = 1;sim <= agent.numSimulations(); sim++) {
		std::cout << "sim: " << sim << std::endl;
		reward_t r = root.sample(agent, 3); // test statement
	}
	// return bestAction(root)
	return agent.genRandomAction(); // test statement
}
