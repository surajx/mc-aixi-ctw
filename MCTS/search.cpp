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
	action_t bestAction(Agent &agent) const {
		std::vector<action_t> best_children;
		best_children.push_back(0);
		double best_mean = children[0]->expectation();
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

	// select action based on UCT algorithm
	action_t selectAction(Agent &agent, int test_arg) const {
		// START test code generate action
		action_t a;
		if (test_arg == 0) {
			a = 0;
		} else if (test_arg == 1) {
			a = 1;
		} else {
			std::cout << "h wrongly identified as previously visited action node" << std::endl;
			a = 0;
		}
		// create child a
		SearchNode ha(true, numPercepts);
		children[a] = &ha;
		return a;
		// END test code generate action

		// START pseudocode
		// make set of unvisited
		// if unvisited nonempty then
		//   for unvisited
		//     pick unif random a
		//     create ha
		//     return a
		// else
		//   return argmax
		// END pseudocode
	}


	// perform a sample run through this node and its children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int horiz, int test_arg) {
		std::cout << "horiz: " << horiz << std::endl;
		std::cout << "m: " << m << std::endl;
		reward_t rew = 0;
		if (horiz == 0) {
			return rew;
		} else if (m_chance_node) {
			// START test code generate percept
			percept_t obs_rew;
			if (test_arg == 2) {
				obs_rew = 0;
			} else if (test_arg == 3) {
				obs_rew = 1;
			} else {
				std::cout << "h wrongly identified as chance node" << std::endl;
				return 0;
			}
			// END test code generate percept
			hor_ptr = new SearchNode(false,numActions);
			if (children[obs_rew] != NULL) { // if child (o,r) does not exist:
				hor_ptr = children[obs_rew];
			}
			rew = rew + hor_ptr->sample(Agent &agent, horiz - 1, test_arg+1);
		} else if (m_visits == 0) {
			rew = playout(agent, horiz);
		} else {
			SearchNode ha = selectAction(Agent &agent, test_arg+1);
			rew = ha.sample(agent, m, test_arg+1);
		}
		// update m_mean
		m_mean =  (1 / m_visits) * (rew + m_visits*m_mean);
		// update visits
		m_visits++;
		return rew;
	}

	// number of times the search node has been visited
	visits_t visits(void) const { return m_visits; }

	// determine the expected reward from this node
	reward_t expectation(void) const { return m_mean; }

private:

	bool m_chance_node; // true if this node is a chance node, false otherwise
	unsigned int m_children; // chance (action) nodes have |X| (|A|) children
	double m_mean;      // the expected reward of this node
	visits_t m_visits;  // number of times the search node has been visited
	std::vector<SearchNode*> children;
};

// simulate a path through a hypothetical future for the agent within its
// internal model of the world, returning the accumulated reward.
static reward_t playout(Agent &agent, unsigned int playout_len) {
	return 1;
}

// number of distinct percepts (size of the percept space)
unsigned int countPercepts(unsigned int num_obs, unsigned num_rew) {
	return (unsigned int) pow(2,num_obs) + pow(2,num_rew);
}

// initialize the tree by setting the constants and creating a root node
void initializeTree(Agent &agent) {
	std::cout << "initializing" << std::endl;
	// numActions = agent.numActions(); // set number of actions
	// numPercepts = countPercepts(agent.numObservations(), agent.numRewards()); // set number of percepts
	numActions = 3; // test
	numPercepts = countPercepts(2,2); // test
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
	std::cout << "test of 1/10: " << 1/10 << std::endl;
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
		reward_t r = root.sample(agent, m, 0); // test statement
	}
	return root.bestAction(agent);
	//return agent.genRandomAction(); // test statement
}
