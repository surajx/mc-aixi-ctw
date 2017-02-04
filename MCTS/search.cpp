#include "search.hpp"
#include "../AIXI/agent.hpp"

#include <iostream>
#include <math.h>
#include <random>

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
			for (int i=0;i<m_children;i++) {
				children[i] = NULL;
			}
			std::cout << "creating search node" << std::endl;
	}

	// return unif random one of the best actions
	action_t selectAction(Agent &agent) const {
		// collect set of best children and return unif randomly one action from set
		std::vector<int> best_children;
		best_children.push_back(0);
		unsigned int best_mean = children[0]->expectation();
		unsigned int num_best_children = 1;
		for (int i = 1; i < m_children;i++) {
			if (children[i] != NULL) {
				if (children[i]->expectation() > children[best_children[0]]->expectation()) { // new best
					best_children = {i};
					best_mean = children[i]->expectation();
					num_best_children = 1;
				} else if (children[i]->expectation() == children[best_children[0]]->expectation()) { // more than one best child
					best_children.push_back(i);
					num_best_children++;
				}
				// if current child is worse, do nothing
			}
		}
		// four lines below (from <random>) generate unif random number between
		// 0 and num_best_children-1 (where the boundary values may be generated as
		// well)
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr(0, num_best_children-1);
		int random_number = distr(generator);
		return best_children[random_number];
	}

	// determine the expected reward from this node
	reward_t expectation(void) const { return m_mean; }

	// perform a sample run through this node and it's children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int dfr) {
		std::cout << "entering sample function call " << std::endl;
		SearchNode child0(true, 2);
		SearchNode child1(true, 2);
		SearchNode child2(true, 2);
		children[0] = &child0;
		children[1] = &child1;
		children[2] = &child2;
		children[0]->m_mean = 4;
		children[1]->m_mean = 4;
		children[2]->m_mean = 4;
		std::cout << "children[1]->expectation(): " << children[1]->expectation() << std::endl;
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

// number of distinct percepts (size of the percept space)
unsigned int countPercepts(unsigned int num_obs, unsigned num_rew) {
	return (unsigned int) pow(2,num_obs) + pow(2,num_rew);
}

// initialize the tree by setting the constants and creating a root node
void initializeTree(Agent &agent) {
	std::cout << "initializing" << std::endl;
	// initialize values
	// numActions = agent.numActions(); // set number of actions
	numActions = 3; // test
	numPercepts = countPercepts(2,2);
	C = agent.exploreExploitRatio();
	m = agent.horizon();
	obsBits = agent.numObservations();
	rewBits = agent.numRewards();
	root_ptr = new SearchNode(false,numActions);
}

// Prune the tree by updating the root node and pruning dead branches
void pruneTree(Agent &agent) {
	std::cout << "pruning" << std::endl;
	//root_ptr = new SearchNode(false,numActions);
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
	std::cout << "root.expectation(): " << root.expectation() << std::endl;
	for (unsigned int sim = 1;sim <= agent.numSimulations(); sim++) {
		std::cout << "sim: " << sim << std::endl;
		reward_t r = root.sample(agent, 3); // test statement
	}
	return root.selectAction(agent);
	//return agent.genRandomAction(); // test statement
}
