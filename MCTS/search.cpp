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
			test_tree_size++; // test
	}

	// recursively destruct the SearchNode and its children.
	// do not destruct the newly assigned root node.
	~SearchNode(void) {
		// if there are children recursively delete all offspring
		test_tree_size--; // test
		for (int i=0;i<m_children;i++) {
			if (children[i] != NULL && children[i] !=  root_ptr) {
				delete children[i];
			}
		}

	}

	// return unif random one of the best actions
	action_t bestAction(Agent &agent) const {
		std::vector<action_t> best_children;
		double best_mean;
		for (action_t i = 0; i < m_children;i++) {
			if (children[i] != NULL) {
				best_children.push_back(i);
				best_mean = children[i]->expectation();
				break;
			}
		}
		unsigned int num_best_children = 1;
		for (action_t i = 0; i < m_children;i++) {
			if (children[i] != NULL) {
				if (children[i]->expectation() > best_mean) { // new best
					best_children = {i};
					best_mean = children[i]->expectation();
					num_best_children = 1;
				} else if (children[i]->expectation() == children[best_children[0]]->expectation() && i != best_children[0]) { // more than one best child
					best_children.push_back(i);
					num_best_children++;
				}
				// if current child is worse, do nothing
			}
		}
		return best_children[randRange(num_best_children)];
	}


	// select action based on UCT
	SearchNode* selectAction(Agent &agent, unsigned int horiz) {
		std::vector<action_t> unvisited;
		for (action_t a = 0; a < numActions; a++) {
			if (children[a] == NULL) {
				unvisited.push_back(a);
			}
		}
		if (unvisited.size() > 0) { // select unvisited action uniformly at random
			int ran_index = randRange(unvisited.size());
			action_t ran_action = unvisited[ran_index];
			// create child a if necessary,
			SearchNode* ha_ptr = new SearchNode(true,numPercepts);
			children[ran_action] = ha_ptr;
			return ha_ptr;
		} else { // find a_max = argmax, return a_max
			action_t a_max = 0;
			double best_val = 0;
			for (action_t a = 0; a < numActions; a++) {
				if (children[a] != NULL) {
					double curr_val = (1.0 / (horiz * (agent.maxReward() - agent.minReward() ))) * children[a]->visits()
						+ C * sqrt(log10(m_visits) / children[a]->visits());
					if (curr_val > best_val) {
						a_max = a;
						best_val = curr_val;
					}
				}
			}
			agent.modelUpdate(a_max);
			return children[a_max];
		}
	}

	// perform a sample run through this node and its children,
	// returning the accumulated reward from this sample run
	reward_t sample(Agent &agent, unsigned int horiz) {
		reward_t rew = 0;
		if (horiz == 0) {
			return rew;
		} else if (m_chance_node) {
			percept_t new_obs = agent.genPerceptAndUpdate(obsBits);
			percept_t new_rew = agent.genPerceptAndUpdate(rewBits);
			percept_t percept_index = perceptIndex(new_obs,new_rew);
			SearchNode* hor_ptr = new SearchNode(false,numActions);
			if (children[percept_index] == NULL) {
				children[percept_index] = hor_ptr;
			} else {
				hor_ptr = children[percept_index];
			}
			rew = new_rew + hor_ptr->sample(agent, horiz - 1);
		} else if (m_visits == 0) {
			rew = playout(agent, horiz);
		} else { // we are at action node with previous visits
			SearchNode* ha_ptr = selectAction(agent, horiz);
			rew = ha_ptr->sample(agent, horiz);
		}
		m_mean =  (1.0 / (m_visits+1)) * (rew + m_visits*m_mean);
		m_visits++;
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
static reward_t playout(Agent &agent, unsigned int horiz) {
	reward_t rew = 0;
	for (int i = 0;i < horiz; i++) {
		action_t act = (action_t) randRange(numActions);
		agent.modelUpdate(act);
		percept_t new_obs = agent.genPerceptAndUpdate(obsBits);
		percept_t new_rew = agent.genPerceptAndUpdate(rewBits);
		rew = rew + new_rew;
	}
	agent.modelRevert();
	return rew;
}

// number of distinct percepts (size of the percept space)
unsigned int countPercepts(unsigned int num_obs, unsigned num_rew) {
	return (unsigned int) pow(2,num_obs) * pow(2,num_rew);
}

// encode an observation and a reward as a single observation-reward
percept_t perceptIndex(percept_t obs, percept_t rew) {
	symbol_list_t obs_bitstring;
	symbol_list_t rew_bitstring;
	encode(obs_bitstring, obs, obsBits);
	encode(rew_bitstring, rew, rewBits);
	obs_bitstring.insert(obs_bitstring.end(),rew_bitstring.begin(),rew_bitstring.end());
	return decode(obs_bitstring, obsBits+rewBits);
}

// initialize the tree by setting the constants and creating a root node
void initializeTree(Agent &agent) {
	numActions = agent.numActions(); // set number of actions
	numPercepts = countPercepts(agent.numObservations(), agent.numRewards()); // set number of percepts
	C = agent.exploreExploitRatio();
	m = agent.horizon();
	obsBits = agent.numObservations();
	rewBits = agent.numRewards();
	root_ptr = new SearchNode(false,numActions);
	test_tree_size = 0; // test
}

// Prune the tree by updating the root node and pruning dead branches
void pruneTree(Agent &agent, percept_t prev_obs, percept_t prev_rew, action_t prev_act) {
	SearchNode* old_root_ptr = root_ptr;
	int prev_obs_rew_index = (int) perceptIndex(prev_obs,prev_rew);
	SearchNode* chance_child = root_ptr->getChildren()[prev_act];
	if (chance_child == NULL) {
		std::cout << "Prune: witness action for which no child existed" << std::endl;
		chance_child = new SearchNode(true,numPercepts);
	}
	SearchNode* action_child = chance_child->getChildren()[prev_obs_rew_index];
	if (action_child == NULL) {
		std::cout << "Prune: witness percept for which no child existed" << std::endl;
		action_child = new SearchNode(false,numActions);
	}
	root_ptr = action_child;
	delete old_root_ptr;
}

// determine the best action by searching ahead using MCTS
extern action_t search(Agent &agent, percept_t prev_obs, percept_t prev_rew, action_t prev_act, bool tree_initialized) {
	std::cout << "Enter search" << std::endl; // testing
	if (!tree_initialized) {
		initializeTree(agent);
	} else {
		pruneTree(agent, prev_obs, prev_rew, prev_act);
	}
	for (unsigned int sim = 1;sim <= agent.numSimulations(); sim++) {
		reward_t r = root_ptr->sample(agent, m);
	}
	std::cout << "Exit search" << std::endl;
	
	std::cout << root_ptr->bestAction(agent) << std::endl;
	return root_ptr->bestAction(agent);
}
