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

	virtual void reset_game();

	virtual void ghost_movement(Ghost ghost);

private:
	int pacmanX;
	int pacmanY;
	int complete_game_state[19][21];
	int original_complete_game_state[19][21];
	int new_position;
	int power_pill;
	int power_pill_time;
	Ghost ghost1;
	Ghost ghost2;
	Ghost ghost3;
	Ghost ghost4;
	int wall;
	int distance;
	int pellat_within_2;
	int pellat_within_3;
	int pellat_within_4;
	int left_food;
	int right_food;
	int up_food;
	int down_food;
	int left_ghost;
	int right_ghost;
	int up_ghost;
	int down_ghost;
	int wall_obs;
	int ghost_obs;
	int smell_obs;
	int pellat_obs;
	int power_obs;
	double random_number;
	int left_wall;
	int right_wall;
	int down_wall;
	int up_wall;
	int food_pellats;

	//more stuff
};


class TicTacToe : public Environment {
public:

	// set up the initial environment percept
	TicTacToe(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

	virtual void reset_game();

private:

	//more stuff
	int state;
	int random_choice;
	int board[3][3];
	std::vector<int> open_squares;
	int win_cond;
	int opponent_win_cond;
	int game_finished;
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

	virtual void reset_game();

private:
	int opponent_card;
	int agent_card;
	int agent_chips_put_in;
	int chips_in_play;
	double alpha;
	int opponent_action;
	//more stuff
};


class RobocupSimulation : public Environment {
public:

	// set up the initial environment percept
	RobocupSimulation(options_t &options);

	// receives the agent's action and calculates the new environment percept
	virtual void performAction(action_t action);

	struct Robot {
		int x;
		int y;
		int body_facing;
		int head_facing;
	};

	struct SoccerBall {
		int x;
		int y;
	};

	virtual void move(int direction, int power, SoccerBall ball, Robot agent);

	virtual void turn_body(int direction, Robot agent);

	virtual void turn_head(int direction, Robot agent);

	virtual void kick(int direction, SoccerBall ball, Robot agent);

	virtual int check_vision(Robot agent, SoccerBall ball);

	

private:
	Robot agent;
	int agent_position_and_heading_and_head[4];
	//int agent_velocity; //probably vector
	SoccerBall ball;
	int ball_position[2];
	int full_field[11][19];
	int home_goal[2];
	int opponent_goal[2];
	int bodyfacing_observation;
	int headfacing_observation;
	int vision_observation;
	int position_observation;
	int ball_vision_number;
	int ball_in_opponent_goals;
	int ball_in_agent_goals;

};



#endif // __ENVIRONMENT_HPP__
