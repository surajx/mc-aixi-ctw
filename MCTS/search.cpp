/****************************************************************************************
** TODO: Documentation
**
** Author: Arie Slobbe
***************************************************************************************/

#include "search.hpp"
#include "../AIXI/agent.hpp"
#include "../common/util.hpp"

#include <iostream>
#include <random>

SearchTree::SearchTree(Agent* ai) {
  numActions = ai->getNumActions();  // set number of actions
  numObservationBits = ai->getNumObservationBits();
  numRewardBits = ai->getNumRewardBits();
  numPercepts = countPercepts(numObservationBits,
                              numRewardBits);  // set number of percepts
  C = ai->getExploreExploitRatio();
  m = ai->horizon();
  rootNode = new SearchNode(ai, false, numActions);
  agent = ai;
}

// simulate a path through a hypothetical future for the agent within its
// internal model of the world, returning the accumulated reward.
reward_t SearchTree::playout(unsigned int searchHorizon) {
  reward_t rew = 0;
  for (int i = 0; i < searchHorizon; i++) {
    action_t act = (action_t)randRange(numActions);
    agent->modelUpdate(act);
    percept_t new_obs = agent->genPerceptAndUpdate(numObservationBits);
    percept_t new_rew = agent->genPerceptAndUpdate(numRewardBits);
    rew += new_rew;
  }
  agent->modelRevert(m);
  return rew;
}

// Prune the tree by updating the root node and pruning dead branches
void SearchTree::pruneTree(percept_t prev_obs,
                           percept_t prev_rew,
                           action_t prev_act) {
  SearchNode* old_root_ptr = rootNode;
  int prev_obs_rew_index =
      (int)perceptIndex(prev_obs, prev_rew, numObservationBits, numRewardBits);
  SearchNode* chance_child = rootNode->getChildren()[prev_act];
  if (chance_child == NULL) {
    chance_child = new SearchNode(agent, true, numPercepts);
  }
  SearchNode* action_child = chance_child->getChildren()[prev_obs_rew_index];
  if (action_child == NULL) {
    action_child = new SearchNode(agent, false, numActions);
  }
  rootNode = action_child;
  delete old_root_ptr;
}

// determine the best action by searching ahead using MCTS
action_t SearchTree::search(percept_t prev_obs,
                            percept_t prev_rew,
                            action_t prev_act) {
  if (!isFirst) {
    pruneTree(prev_obs, prev_rew, prev_act);
  } else {
    isFirst = false;
  }
  for (unsigned int sim = 1; sim <= agent->numSimulations(); sim++) {
    reward_t r = rootNode->sample(m);
  }
  std::cout << "Best Action: " << rootNode->bestAction() << std::endl;
  std::cout << "Search Node Count: " << SearchNode::node_count << std::endl;
  return rootNode->bestAction();
}

/**
* SearchNode Implementation.
*/
SearchNode::SearchNode(Agent* ai,
                       bool is_chance_node,
                       unsigned int num_children) {
  m_visits = 0;
  m_mean = 0;
  m_children = num_children;
  m_chance_node = is_chance_node;
  children.resize(m_children);
  for (int i = 0; i < m_children; i++) {
    children[i] = NULL;
  }

  numActions = ai->getNumActions();  // set number of actions
  numObservationBits = ai->getNumObservationBits();
  numRewardBits = ai->getNumRewardBits();
  numPercepts = countPercepts(numObservationBits,
                              numRewardBits);  // set number of percepts
  C = ai->getExploreExploitRatio();
  m = ai->horizon();

  agent = ai;
  node_count += 1;
}

uint_t SearchNode::node_count = 0;

SearchNode::~SearchNode(void) {
  // if there are children recursively delete all offspring
  for (int i = 0; i < m_children; i++) {
    if (children[i] != NULL && children[i] != agent->getPlannerRootNode()) {
      delete children[i];
    }
  }

  node_count -= 1;
}

// return unif random one of the best actions
action_t SearchNode::bestAction() const {
  std::vector<action_t> best_children;
  double best_mean;
  for (action_t i = 0; i < m_children; i++) {
    if (children[i] != NULL) {
      best_children.push_back(i);
      best_mean = children[i]->expectation();
      break;
    }
  }
  unsigned int num_best_children = 1;
  for (action_t i = 0; i < m_children; i++) {
    if (children[i] != NULL) {
      if (children[i]->expectation() > best_mean) {  // new best
        best_children = {i};
        best_mean = children[i]->expectation();
        num_best_children = 1;
      } else if (children[i]->expectation() ==
                     children[best_children[0]]->expectation() &&
                 i != best_children[0]) {  // more than one best child
        best_children.push_back(i);
        num_best_children++;
      }
      // if current child is worse, do nothing
    }
  }
  return best_children[randRange(num_best_children)];
}

// select action based on UCT
SearchNode* SearchNode::selectAction(unsigned int horiz) {
  std::vector<action_t> unvisited;
  for (action_t a = 0; a < numActions; a++) {
    if (children[a] == NULL) {
      unvisited.push_back(a);
    }
  }
  if (unvisited.size() > 0) {  // select unvisited action uniformly at random
    int ran_index = randRange(unvisited.size());
    action_t ran_action = unvisited[ran_index];
    SearchNode* ha_ptr = new SearchNode(agent, true, numPercepts);
    children[ran_action] = ha_ptr;
    agent->modelUpdate(ran_action);
    return ha_ptr;
  } else {  // find a_max = argmax, return a_max
    action_t a_max = 0;
    double best_val = 0;
    for (action_t a = 0; a < numActions; a++) {
      if (children[a] != NULL) {
        double curr_val =
            (1.0 / (horiz * (agent->maxReward() - agent->minReward()))) *
                children[a]->expectation() +
            C * sqrt(log10(m_visits) / children[a]->visits());
        if (curr_val > best_val) {
          a_max = a;
          best_val = curr_val;
        }
      }
    }
    agent->modelUpdate(a_max);
    return children[a_max];
  }
}

// perform a sample run through this node and its children,
// returning the accumulated reward from this sample run
reward_t SearchNode::sample(unsigned int horiz) {
  reward_t rew = 0;
  if (horiz == 0) {
    agent->modelRevert(m);
    return rew;
  } else if (m_chance_node) {
    percept_t new_obs = agent->genPerceptAndUpdate(numObservationBits);
    percept_t new_rew = agent->genPerceptAndUpdate(numRewardBits);
    percept_t percept_index =
        perceptIndex(new_obs, new_rew, numObservationBits, numRewardBits);
    SearchNode* hor_ptr;
    if (children[percept_index] == NULL) {
      hor_ptr = new SearchNode(agent, false, numActions);
      children[percept_index] = hor_ptr;
    } else {
      hor_ptr = children[percept_index];
    }
    rew = new_rew + hor_ptr->sample(horiz - 1);
  } else if (m_visits == 0) {
    rew = agent->getPlanner()->playout(horiz);
  } else {  // we are at action node with previous visits
    SearchNode* ha_ptr = selectAction(horiz);
    rew = ha_ptr->sample(horiz);
  }
  m_mean = (1.0 / (m_visits + 1)) * (rew + m_visits * m_mean);
  m_visits++;
  return rew;
}
