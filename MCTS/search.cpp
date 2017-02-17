#include "search.hpp"
#include "../AIXI/agent.hpp"
#include "../common/util.hpp"

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
			m_chance_node = is_chance_node;
			children.resize(m_children);
			for (int i=0;i<m_children;i++) {
				children[i] = NULL;
			}
			// START block for testing
			if (m_chance_node) {
				// std::cout << "creating chance node" << std::endl;
			} else {
				// std::cout << "creating action node" << std::endl;
			}
			// END block for testing
	}

	// recursively destruct the SearchNode and its children.
	// do not destruct the newly assigned root node.
	~SearchNode(void) {
		// if there are children recursively delete all offspring
		// std::cout << "delete node with m_visits: " << m_visits << std::endl;
		for (int i=0;i<m_children;i++) {
			if (children[i] != NULL && children[i] !=  root_ptr) {
				delete children[i];
			}
		}

	}

	// return unif random one of the best actions
	action_t bestAction(Agent &agent) const {
		std::vector<action_t> best_children;
		best_children.push_back(0);
		double best_mean = children[0]->expectation();
		unsigned int num_best_children = 1;
		for (action_t i = 1; i < m_children;i++) {
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
		return best_children[randRange(num_best_children)];
	}


	// select action based on UCT
	SearchNode* selectAction(Agent &agent, unsigned int horiz, int test_arg) {
		std::vector<action_t> unvisited;
		for (action_t a = 0; a < numActions; a++) {
			if (children[a] == NULL) {
				// std::cout << "selectAction finds unvisited action: " << a << std::endl;
				unvisited.push_back(a);
			}
		}
		if (unvisited.size() > 0) { // select unvisited action uniformly at random
			int ran_index = randRange(unvisited.size());
			action_t ran_action = unvisited[ran_index];
			// create child a if necessary,
			// std::cout << "selectAction selects unvisited action: " << ran_action << std::endl;
			SearchNode* ha_ptr = new SearchNode(true,numPercepts);
			children[ran_action] = ha_ptr;
			return ha_ptr;
		} else { // find a_max = argmax, return a_max
			// std::cout << "selectAction finds all actions were visited." << std::endl;
			action_t a_max = 0;
			double best_val = 0;
			for (action_t a = 0; a < numActions; a++) {
				if (children[a] != NULL) {
					// std::cout << "selectAction finding argmax. a: " << a << ", children[a]: " << children[a] << std::endl;
					double curr_val = (1.0 / (horiz * (agent.maxReward() - agent.minReward() ))) * children[a]->visits()
						+ C * sqrt(log10(m_visits) / children[a]->visits());
					// std::cout << "curr_val: " << curr_val << std::endl;
					if (curr_val > best_val) {
						a_max = a;
						best_val = curr_val;
						// std::cout << "new best action " << a_max << " with value " << best_val << std::endl;
					}
				}
			}
			std::cout << "call modelUpdate from selectAction" << std::endl;
			agent.modelUpdate(a_max);
			return children[a_max];
		}
	}

	// perform a sample run through this node and its children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int horiz, int test_arg) {
		// std::cout << "Enter sample. horiz: " << horiz << std::endl;
		// START part of testing
		if (test_arg == 4) {
			test_arg = 2;
		}
		// END part of testing
		// std::cout << "test_arg: " << test_arg << std::endl;
		// std::cout << "m_chance_node: " << m_chance_node << std::endl;
		reward_t rew = 0;
		if (horiz == 0) {
			return rew;
		} else if (m_chance_node) {
			// std::cout << "call modelUpdate from sample" << std::endl;
			percept_t new_obs = agent.genPerceptAndUpdate(obsBits);
			percept_t new_rew = agent.genPerceptAndUpdate(rewBits);
			// // START test code generate percept
			// percept_t new_obs;
			// percept_t new_rew;
			// if (test_arg == 1 || test_arg == 3) {
			// 	new_obs = 1;
			// 	new_rew = 1;
			// } else if (test_arg == 2) {
			// 	new_obs = 1;
			// 	new_rew = 1;
			// } else {
			// 	// std::cout << "h wrongly identified as chance node" << std::endl;
			// 	new_obs = 0;
			// 	new_rew = 0;
			// }
			// percept_t percept_index = perceptIndex(new_obs,new_rew);
			// // std::cout << "new_obs new_rew: " << new_obs << new_rew << ", percept_index: " << percept_index << std::endl;
			// // END test code generate percept

			percept_t percept_index = perceptIndex(new_obs,new_rew);

			SearchNode* hor_ptr = new SearchNode(false,numActions);
			if (children[percept_index] == NULL) {
				children[percept_index] = hor_ptr;
				// std::cout << "Percept witnessed for first time" << std::endl;
			} else {
				hor_ptr = children[percept_index];
				// std::cout << "Percept witnessed before" << std::endl;
			}
			rew = new_rew + hor_ptr->sample(agent, horiz - 1, test_arg);
		} else if (m_visits == 0) {
			rew = playout(agent, horiz, test_arg);
		} else { // we are at action node with previous visits
			SearchNode* ha_ptr = selectAction(agent, horiz, test_arg);
			rew = ha_ptr->sample(agent, horiz, test_arg);
		}
		m_mean =  (1.0 / (m_visits+1)) * (rew + m_visits*m_mean);
		m_visits++;
		// std::cout << "End sample. horiz: " << horiz << " and m_chance_node: " << m_chance_node << std::endl;
		// std::cout << "m_mean: " << m_mean << ", m_visits: " << m_visits << ", rew: " << rew << std::endl;
		return rew;
	}

	// number of times the search node has been visited
	visits_t visits(void) const { return m_visits; }

	// determine the expected reward from this node
	reward_t expectation(void) const { return m_mean; }

	// vector with children
	std::vector<SearchNode*> getChildren(void) const { return children; }

private:

	bool m_chance_node; // true if this node is a chance node, false if action
	unsigned int m_children; // chance (action) nodes have |X| (|A|) children
	double m_mean;      // the expected reward of this node
	visits_t m_visits;  // number of times the search node has been visited
	std::vector<SearchNode*> children;
};

// simulate a path through a hypothetical future for the agent within its
// internal model of the world, returning the accumulated reward.
static reward_t playout(Agent &agent, unsigned int horiz, int test_arg) {
	reward_t rew = 0;
	rew = rew + test_arg; // test
	// std::cout << "Executing playout. test_arg: " << test_arg << std::endl;
	for (int i = 0;i < horiz; i++) {
		action_t act = (action_t) randRange(numActions);
		// std::cout << "call modelUpdate from playout" << std::endl;
		agent.modelUpdate(act);
		// std::cout << "call genPerceptAndUpdate from playout" << std::endl;
		percept_t new_obs = agent.genPerceptAndUpdate(obsBits);
		percept_t new_rew = agent.genPerceptAndUpdate(rewBits);
		rew = rew + new_rew;
	}	
	agent.modelRevert();
	// std::cout << "af Cycle "  << std::endl;
	return rew;
}

// number of distinct percepts (size of the percept space)
unsigned int countPercepts(unsigned int num_obs, unsigned num_rew) {
	return (unsigned int) pow(2,num_obs) * pow(2,num_rew);
}

// encode an observation and a reward as a single observation-reward
percept_t perceptIndex(percept_t obs, percept_t rew) {
	// return pow(2,obsBits) * obs + rew;
	symbol_list_t obs_bitstring;
	symbol_list_t rew_bitstring;
	encode(obs_bitstring, obs, obsBits);
	encode(rew_bitstring, rew, rewBits);
	obs_bitstring.insert(obs_bitstring.end(),rew_bitstring.begin(),rew_bitstring.end());
	return decode(obs_bitstring, obsBits+rewBits);
}

// initialize the tree by setting the constants and creating a root node
void initializeTree(Agent &agent) {
	// std::cout << "initializing" << std::endl;
	numActions = agent.numActions(); // set number of actions
	numPercepts = countPercepts(agent.numObservations(), agent.numRewards()); // set number of percepts
	// std::cout << "numActions: " << numActions << ", numPercepts: " << numPercepts << std::endl;
	C = agent.exploreExploitRatio();
	m = agent.horizon();
	obsBits = agent.numObservations();
	rewBits = agent.numRewards();
	root_ptr = new SearchNode(false,numActions);
}

// Prune the tree by updating the root node and pruning dead branches
void pruneTree(Agent &agent, percept_t prev_obs, percept_t prev_rew, action_t prev_act) {
	SearchNode* old_root_ptr = root_ptr;
	int prev_obs_rew_index = (int) perceptIndex(prev_obs,prev_rew);
	// std::cout << "pruning with prev_act: " << prev_act << ", prev_obs_rew_index: " << prev_obs_rew_index << std::endl;
	SearchNode* chance_child = root_ptr->getChildren()[prev_act];
	if (chance_child == NULL) {
		// std::cout << "chance child did not exist." << std::endl;
		chance_child = new SearchNode(true,numPercepts);
	}
	SearchNode* action_child = chance_child->getChildren()[prev_obs_rew_index];
	if (action_child == NULL) {
		// std::cout << "action child did not exist." << std::endl;
		action_child = new SearchNode(false,numActions);
	}
	root_ptr = action_child;
	//root_ptr = root_ptr->getChildren()[prev_act]->getChildren()[prev_obs_rew_index];
	// root_ptr = root_ptr->getChildren()[1]->getChildren()[1]; // test
	// std::cout << "setting new root address: " << root_ptr << std::endl;
	delete old_root_ptr;
}

// determine the best action by searching ahead using MCTS
extern action_t search(Agent &agent, percept_t prev_obs, percept_t prev_rew, action_t prev_act, bool tree_initialized) {
	// std::cout << "Start search. lifetime: " << agent.lifetime() << std::endl;
	if (!tree_initialized) {
		initializeTree(agent);
	} else {
		pruneTree(agent, prev_obs, prev_rew, prev_act);
	}
	// std::cout << "agent.numSimulations(): " << agent.numSimulations() << std::endl;
	for (unsigned int sim = 1;sim <= agent.numSimulations(); sim++) {
		// std::cout << "sim: " << sim << std::endl;
		reward_t r = root_ptr->sample(agent, m, sim-1); // test statement
	}
	// std::cout << "End search. obsBits rewBits: " << obsBits << rewBits << std::endl;
	return 1; // test
	// std::cout << "End search. Return: " << root_ptr->bestAction(agent) << std::endl;
	//return root_ptr->bestAction(agent);
}
