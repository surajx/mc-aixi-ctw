#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "../common/util.hpp"

CoinFlip::CoinFlip(options_t &options) {
	// Determine the probability of the coin landing on heads
	p = 1.0;
	if (options.count("coin-flip-p") > 0) {
		strExtract(options["coin-flip-p"], p);
	}
	assert(0.0 <= p);
	assert(p <= 1.0);

	// Set up the initial observation
	m_observation = rand01() < p ? 1 : 0;
	m_reward = 0;
}

// Observes 1 (heads) with probability p and 0 (tails) with probability 1 - p.
// Observations are independent of the agent's actions. Gives a reward of 1 if
// the agent correctly predicts the next observation and 0 otherwise.
void CoinFlip::performAction(action_t action) {
	m_observation = rand01() < p ? 1 : 0;
	m_reward = action == m_observation ? 1 : 0;
}


CTWTest::CTWTest(options_t &options) {
	//Also stuff
	cycle = 0;
	// Set up the initial observation
	//Stuff
	m_observation = 0;
	m_reward = 0;
}


void CTWTest::performAction(action_t action) {
	//Observations
	// Check if the cycle is 0 or not
	// Reward is 1 if observations match actions, 0 otherwise
	// Next Observation is inverse of reward
	if (cycle == 0) {
		m_observation = 0;
		m_reward = (m_observation == action) ? 1 : 0;
	} else {
		m_observation = (m_last_action + 1) % 2;
		m_reward = (m_observation == action) ? 1 : 0;
	}
	m_last_action = action;
	cycle += 1;
}





Pacman::Pacman(options_t &options) {
	//Also stuff
	ghost1 = {8, 9, 5, 1};
	ghost2 = {8, 10, 5, 1};
	ghost3 = {9, 9, 6, 1};
	ghost4 = {9, 10, 4, 1};
	pacmanX = 9;
	pacmanY = 12;
	power_pill = 0;
	// Setting outer walls in game state, 1 is for walls
	for (int i=0;i<19;i++){
		complete_game_state[i][0] = complete_game_state[i][20] = complete_game_state[0][i] = complete_game_state[18][i] = 1;
	}
	complete_game_state[0][19] = complete_game_state[0][20] = complete_game_state[18][19] = complete_game_state[18][20] = 1;
	//complete_game_state[]
	//TODO add all the walls

	//TODO add random bits

	// Set up the initial observation
	//Stuff
}


void Pacman::performAction(action_t action) {
	
	//ghost movement
	//check manhatten distance
	

	// check if distance is less then or equal to 5
	if (ghost1.manhattan_distance <= 5) {
		//pursue pacman
	} else {
		//random move
	}
	if (ghost2.manhattan_distance <= 5) {
		//pursue pacman
	} else {
		//random move
	}
	if (ghost3.manhattan_distance <= 5) {
		//pursue pacman
	} else {
		//random move
	}
	if (ghost4.manhattan_distance <= 5) {
		//pursue pacman
	} else {
		//random move
	}

	// and pacman movement
	if (action == 1){
		new_position = complete_game_state[pacmanX+1][pacmanY];
	} else if (action == 2){
		new_position = complete_game_state[pacmanX-1][pacmanY];
	} else if (action == 3){
		new_position = complete_game_state[pacmanX][pacmanY+1];
	} else {
		new_position = complete_game_state[pacmanX][pacmanY-1];
	}
	switch(new_position) {
		case 0:
			//the new position is a blank space, pacman moves into it
			if (action == 1){
				pacmanX += 1;
			} else if (action == 2){
				pacmanX -= 1;
			} else if (action == 3){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			m_reward -= 1;
		case 1:
			//The new position is a wall, and pacman can't move into it
			//pacmanX = pacmanX;
			//pacmanY = pacmanY;
			m_reward -= 10;
			break;
		case 2:
			//The new position is a bit
			if (action == 1){
				pacmanX += 1;
			} else if (action == 2){
				pacmanX -= 1;
			} else if (action == 3){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			m_reward += 10;
			break;
	}

	
	//check if caught by ghosts
	if ((ghost1.x == pacmanX && ghost1.y == pacmanY) || (ghost2.x == pacmanX && ghost2.y == pacmanY) || (ghost3.x == pacmanX && ghost3.y == pacmanY) || (ghost4.x == pacmanX && ghost4.y == pacmanY)) {
		if (power_pill) {
			//
		} else {
			m_reward -= 50;
			//game ends
		}
	} else if ((ghost1.manhattan_distance == 1 && (ghost1.action + action == 3 || ghost1.action + action == 7)  && (abs(ghost1.x -pacmanX) + abs(ghost1.y -pacmanY) == 1)) || (ghost2.manhattan_distance == 1 && (ghost2.action + action == 3 || ghost2.action + action == 7)  && (abs(ghost2.x -pacmanX) + abs(ghost2.y -pacmanY) == 1)) || (ghost3.manhattan_distance == 1 && (ghost3.action + action == 3 || ghost3.action + action == 7)  && (abs(ghost3.x -pacmanX) + abs(ghost3.y -pacmanY) == 1)) || (ghost4.manhattan_distance == 1 && (ghost4.action + action == 3 || ghost4.action + action == 7) && (abs(ghost4.x -pacmanX) + abs(ghost4.y -pacmanY) == 1))) {
		//check if moved through any of the ghosts
		// by checking if old and current manhattan_distance is 1 and both pacman and ghost both moved in opposite directions in the same axis
		if (power_pill) {
			//
		} else {
			m_reward -= 50;
			//game ends
		}
		
	}
	// calculate mantahhan distances
	ghost1.manhattan_distance = abs(ghost1.x -pacmanX) + abs(ghost1.y -pacmanY);
	ghost2.manhattan_distance = abs(ghost2.x -pacmanX) + abs(ghost2.y -pacmanY);
	ghost3.manhattan_distance = abs(ghost3.x -pacmanX) + abs(ghost3.y -pacmanY);
	ghost4.manhattan_distance = abs(ghost4.x -pacmanX) + abs(ghost4.y -pacmanY);


	//Resolve Observations

	// 4-bit describing walls
	//complete_game_state[pacmanX+1][pacmanY] % 2;
	//complete_game_state[pacmanX-1][pacmanY] % 2;
	//complete_game_state[pacmanX][pacmanY+1] % 2;
	//complete_game_state[pacmanX][pacmanY-1] % 2;

	// 4-bit describing whether or not ghost is visible

	// 3-bit observation on whether or not a pellat is within 2,3,4 manhattan distance from pacman

	// 4-bit describing whether of not food is in line of sight

	// 1-bit describing whether or not pacman is under effects of power pill
}





TicTacToe::TicTacToe(options_t &options) {
	//Also stuff
	state = pow(2,19);
	// Set up the initial observation
	//Stuff
}


void TicTacToe::performAction(action_t action) {
	//Agent move is added to state
	//If he wins game ends, if illegal lose points
	// All positive rewards???
	if ( (fmod((state/pow(4, action)),2) == 1) || (fmod((state/(pow(4, action)*2) ),2) == 1) ) {
		m_reward += 0;
	} else {
		state += pow(4,action);
		if (isFinished()) {
			m_reward += 5;
		} else {
			// find how many moves are possible, then make random move
			random_choice = 0;
			state += pow(4,random_choice)*2;
			if (isFinished()) {
				m_reward += 1;
			}
		}
	}
	m_observation = state;
	//Random Opponent makes moves
	//if he wins game ends, if not give observations

}


BiasedRockPaperSciessor::BiasedRockPaperSciessor(options_t &options) {
	//Also stuff

	// Set up the initial observation
	//Stuff
}


void BiasedRockPaperSciessor::performAction(action_t action) {
	//Observations
	// 0 is rock
	// 1 is paper
	// 2 is scisors
	// Opponent chooses last rounds action if they won
	// Or random action otherwise
	if (opponent_won_last_round) {
		opponent_action = opponent_last_round_action;
	} else {
		opponent_action = (rand01() < (1.0/3.0) ) ? ((rand01() < 0.5 ) ? 2 : 1) : 0;
	}

	// If same actions then draw, if agent wins +1 reward, if looses -1 reward
	if (opponent_action == action) {
		m_reward += 0;
		m_observation = opponent_action;
		opponent_won_last_round = 0;
	} else if ((opponent_action == 0 && action == 1) || (opponent_action == 1 && action == 2) || (opponent_action == 2 && action == 0)) {
		m_reward += 1;
		m_observation = opponent_action;
		opponent_won_last_round = 0;
	} else {
		m_reward -= 1;
		m_observation = opponent_action;
		opponent_won_last_round = 1;
	}
}


ExtendedTiger::ExtendedTiger(options_t &options) {
	//Initial state of 0 is sitting, 1 is standing
	state = 0;
	// Set up the initial observation

	//Setting up initial reward
	m_observation = 0;
	m_reward = 0;
	gold_door = (rand01() < 0.5) ? 1 : 2;
	tiger_door = ((gold_door + 1) % 2) + 1;
}


void ExtendedTiger::performAction(action_t action) {
	//Observations
	// Actions
	// 1 is stand
	// 2 is listen
	// 3 is open door 1
	// 4 is open door 2
	switch(action){
		//stuff
		case 1:
			//stuff
			if (state == 1) {
				m_reward += 90;
			} else{
				state = 1;
				m_reward += 99;
			}
			break;
		case 2:
			//stuff
			if (state == 1) {
				m_reward += 90;
			} else{
				m_observation = (rand01() < 0.85) ? tiger_door : gold_door;
				m_reward += 99;
			}
			break;
		case 3:
			//stuff
			if (state == 0) {
				m_reward += 90;
			} else {
				m_reward += (1 == gold_door) ? 130 : 0;
			}
			break;
		case 4:
			//stuff
			if (state == 0) {
				m_reward += 90;
			} else {
				m_reward += (2 == gold_door) ? 130 : 0;
			}
			break;
	}

	/*
	if (state == 0 && (action ==  'open door 1' || action == 'open door 2')) {
		m_reward -= 10;
	} else if (state == 0) {
		m_reward -= 1;
		state = (action == 'stand') ? 1 : 0;
		m_observation = (action == 'listen') ? (rand01() < 0.85 ? tiger_door : gold_door) : 0;
	} else if (action == 'stand' || action == 'listen') {
		m_reward -= 10;
	} else {
		m_reward = (action == 'open tiger door') ? -100 : 30;
		m_observation = (action == 'open tiger door') ? 'tiger' : 'gold';
	} */
}


KuhnPoker::KuhnPoker(options_t &options) {
	//Also stuff

	m_reward = 0;
	// Players are delt cards
	// 2 is king, 1 is queen, 0 is jack
	// First opponent is delt a card
	opponent_card  = rand01() < 1/3.0 ? 2 : (rand01() < 0.5 ? 1 : 0);

	// agent is delt card
	agent_card  = rand01() < 0.5 ? ((opponent_card + 1 )%3) : ((opponent_card + 2 )%3);
	

	// Both parties bet
	agent_chips_put_in = 1;
	chips_in_play = 2;


	// Opponent takes action, pass or bet, using a nash stratergy
	// 1 is to bet, 0 is to pass
	// for the nash stratergy, first choose any alpha between 0 and 1/3
	// this is the probability the opponent bets on a jack
	// the opponent will always pass on a queen
	// and bet with a probabiliy of 3alpha with a king
	alpha = rand01() / 3.0;
	if (opponent_card == 0) {
		if (rand01() < alpha) {
			opponent_action = 1;
			chips_in_play += 1;
		} else {
			opponent_action = 0;
		}
	} else if (opponent_card == 1) {
		opponent_action = 0;
	} else {
		if (rand01() < 3*alpha) {
			opponent_action = 1;
			chips_in_play += 1;
		} else {
			opponent_action = 0;
		}
	}

	// observations are current card, and opponents choice (to bet or pass)
	// 3 bits on agent card?, 1 bit on opponent actions
	m_observation = (opponent_action ? pow(2,3): 0) + ((agent_card < 2) ? ((agent_card < 1) ? pow(2,1): pow(2,0)) : pow(2,2));
}


void KuhnPoker::performAction(action_t action) {

	// Agent takes action, 1 to bet or 0 to pass

	// amount of chips put in, and amoutn of chips in play both increase if agent bets
	agent_chips_put_in += action;
	chips_in_play += action;


	// if actions are equal showdown occurs, 
	//else if opponents bets he wins, 
	//else opponent has choice to bet and showdown occurs or pass and let agent win

	if (opponent_action == action) {
		//showdown
		if (agent_card > opponent_card) {
			//agent wins
			m_reward = chips_in_play;
		} else {
			//opponent wins
			m_reward = -agent_chips_put_in; 
		}

	} else if (opponent_action == 1) {
		//opponent wins
		m_reward = -agent_chips_put_in; 
	} else {
		if (rand01() < alpha + 1/3.0) {
			// opponent calls then showdown
			chips_in_play += 1;
			if (agent_card > opponent_card) {
				//agent wins
				m_reward = chips_in_play;
			} else {
				//opponent wins
				m_reward = -agent_chips_put_in; 
			}

		} else {
			//opponent folds and agent wins
			m_reward = chips_in_play;
		}
	}
	
}

