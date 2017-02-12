#ifndef __ENVIRONMENT_HPP__
#define __ENVIRONMENT_HPP__

#include "../common/types.hpp"

class Environment {

public:

	// Constructor: set up the initial environment percept
	// TODO: implement in inherited class

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action) = 0; // TODO: implement in inherited class

	// returns true if the environment cannot interact with the agent anymore
	virtual bool isFinished(void) const { return false; } // TODO: implement in inherited class (if necessary)

	void getPercept(symbol_list_t &symbol_list);

	percept_t getObservation(void) const { return m_observation; }

	percept_t getReward(void) const { return m_reward; }

protected: // visible to inherited classes
	action_t m_last_action;  // the last action performed by the agent
	percept_t m_observation; // the current observation
	percept_t m_reward;      // the current reward

};


// An experiment involving flipping a biased coin and having the agent predict
// whether it will come up heads or tails. The agent receives a reward of 1
// for a correct guess and a reward of 0 for an incorrect guess.
class CoinFlip : public Environment {
public:

	// set up the initial environment percept
	CoinFlip(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:
	double p; // Probability of observing 1 (heads)
};


class CTWTest : public Environment {
public:

	// set up the initial environment percept
	CTWTest(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:
	int cycle;
	//more stuff
};







class Pacman : public Environment {
public:

	// set up the initial environment percept
	Pacman(options_t &options);

	// Ghost struct
	struct Ghost {
		int x;
		int y;
		int manhattan_distance;
		int action;
	};

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:
	int pacmanX;
	int pacmanY;
	int complete_game_state[19][21];
	int new_position;
	int power_pill;
	Ghost ghost1;
	Ghost ghost2;
	Ghost ghost3;
	Ghost ghost4;

	//more stuff
};


class TicTacToe : public Environment {
public:

	// set up the initial environment percept
	TicTacToe(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:

	//more stuff
	int state;
	int random_choice;
};


class BiasedRockPaperSciessor : public Environment {
public:

	// set up the initial environment percept
	BiasedRockPaperSciessor(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:
	int opponent_action;
	int opponent_won_last_round;
	int opponent_last_round_action;
	//more stuff
};


class ExtendedTiger : public Environment {
public:

	// set up the initial environment percept
	ExtendedTiger(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:
	// Initial state is sitting
	int state;
	int tiger_door;
	int gold_door;
	//more stuff
};



class KuhnPoker : public Environment {
public:

	// set up the initial environment percept
	KuhnPoker(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

private:

	//more stuff
};






#endif // __ENVIRONMENT_HPP__
