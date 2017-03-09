#include <cassert>
#include <deque>

#include "../CTW/ContextTree.hpp"
#include "../MCTS/search.hpp"
#include "../common/util.hpp"

#include "agent.hpp"

// construct a learning agent from the command line arguments
Agent::Agent(options_t& options) {
  std::string s;

  strExtract(options["agent-actions"], numActions);
  strExtract(options["agent-horizon"], searchHorizon);
  strExtract(options["observation-bits"], numObervationBits);
  strExtract(options["num-simulations"], numSimPerCycle);
  strExtract(options["exploration-exploitation-ratio"], exploreExploitRatio);
  strExtract<unsigned int>(options["reward-bits"], numRewardBits);

  // Calculate the number of bits needed to represent the action
  for (unsigned int i = 1, c = 1; i < numActions; i *= 2, c++) {
    numActionBits = c;
  }

  contextTree = new ContextTree(strExtract<unsigned int>(options["ct-depth"]));
  reset();
}

Agent::~Agent(void) {
  if (contextTree)
    delete contextTree;
}

bool Agent::initPlanner() {
  planner = new SearchTree(this);
  if (planner)
    return true;
  return false;
}

action_t Agent::getPlannedAction(percept_t prev_obs,
                                 percept_t prev_rew,
                                 action_t prev_act) {
  return planner->search(prev_obs, prev_rew, prev_act);
}

SearchTree* Agent::getPlanner() {
  return planner;
}

SearchNode* Agent::getPlannerRootNode() {
  return planner->getRootNode();
}

lifetime_t Agent::lifetime(void) const {
  return agentAge;
}

reward_t Agent::reward(void) const {
  return totalReward;
}

reward_t Agent::averageReward(void) const {
  return lifetime() > 0 ? reward() / reward_t(lifetime()) : 0.0;
}

reward_t Agent::maxReward(void) const {
  return reward_t((1 << numRewardBits) - 1);
}

reward_t Agent::minReward(void) const {
  return 0.0;
}

unsigned int Agent::getNumObservationBits(void) const {
  return numObervationBits;
}

unsigned int Agent::getNumRewardBits(void) const {
  return numRewardBits;
}

unsigned int Agent::getNumActionBits(void) const {
  return numActionBits;
}

unsigned int Agent::getNumActions(void) const {
  return numActions;
}

unsigned int Agent::numSimulations(void) const {
  return numSimPerCycle;
}

unsigned int Agent::getExploreExploitRatio(void) const {
  return exploreExploitRatio;
}

size_t Agent::historySize(void) const {
  return contextTree->historySize();
}

size_t Agent::horizon(void) const {
  return searchHorizon;
}

action_t Agent::genRandomAction(void) const {
  return randRange(numActions);
}

percept_t Agent::genPercept(uint_t percept_size) const {
  symbol_list_t percept;
  contextTree->genNextSymbols(percept, percept_size);
  return decode(percept, percept_size);
}

percept_t Agent::genPerceptAndUpdate(uint_t percept_size) {
  symbol_list_t percept;
  contextTree->genNextSymbolsAndUpdate(percept, percept_size);
  isLastUpdateAPercept = true;
  return decode(percept, percept_size);
}

void Agent::modelUpdate(percept_t observation, percept_t reward) {
  // Update internal model
  symbol_list_t percept;
  encodePercept(percept, observation, reward);
  contextTree->update(percept);

  // Update other properties
  totalReward += reward;
  isLastUpdateAPercept = true;
}

void Agent::modelUpdate(action_t action) {
  assert(isActionOk(action));
  assert(isLastUpdateAPercept == true);

  // Update internal model
  symbol_list_t action_syms;
  encodeAction(action_syms, action);

  // Only updating the CTW history as it is AC-CTW
  // Model update is not needed when observing an action.
  contextTree->updateHistory(action_syms);

  agentAge++;
  isLastUpdateAPercept = false;
}

bool Agent::modelRevert(uint_t newSearchHorizon) {
  for (size_t simIdx = 0; simIdx < newSearchHorizon; simIdx++) {
    for (size_t perceptBit = 0; perceptBit < numObervationBits + numRewardBits;
         perceptBit++) {
      contextTree->revert();
    }
    for (size_t actionBit = 0; actionBit < numActionBits; actionBit++) {
      contextTree->revertHistory();
    }

    // After reverting one obs-rew-act triple, set lifetime back by 1
    agentAge--;
  }
}

void Agent::reset(void) {
  contextTree->clear();

  agentAge = 0;
  totalReward = 0.0;
}

// action sanity check
bool Agent::isActionOk(action_t action) const {
  return action < numActions;
}

// reward sanity check
bool Agent::isRewardOk(reward_t reward) const {
  return reward >= minReward() && reward <= maxReward();
}

// Encodes an action as a list of symbols
void Agent::encodeAction(symbol_list_t& symlist, action_t action) const {
  symlist.clear();

  encode(symlist, action, numActionBits);
}

// Encodes a percept (observation, reward) as a list of symbols
void Agent::encodePercept(symbol_list_t& symlist,
                          percept_t observation,
                          percept_t reward) const {
  symlist.clear();

  encode(symlist, observation, numObervationBits);
  encode(symlist, reward, numRewardBits);
}

// Decodes the observation from a list of symbols
action_t Agent::decodeAction(const symbol_list_t& symlist) const {
  return decode(symlist, numActionBits);
}

// Decodes the reward from a list of symbols
percept_t Agent::decodeReward(const symbol_list_t& symlist) const {
  return decode(symlist, numRewardBits);
}
