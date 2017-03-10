#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <algorithm>


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


void Pacman::reset_game() {
	for (int i=0;i<19;i++){
		for (int j=0;j<21;j++) {
			complete_game_state[i][j] = 0;
		}
	}
	// Reset the pacman game
	ghost1 = {9, 8, 7, 1, 9, 8};
	ghost2 = {10, 8, 7, 1, 10, 8};
	ghost3 = {9, 9, 7, 1, 9, 9};
	ghost4 = {10, 9, 7, 1, 10, 9};
	// ghosts also saving in game state as 4
	complete_game_state[9][8] = 4;
	complete_game_state[10][8] = 4;
	complete_game_state[9][9] = 4;
	complete_game_state[10][9] = 4;

	// game state of 6 is power pill
	// game state of 8 is ghost and food
	// game state of 10 is power pill and ghost

	pacmanX = 9;
	pacmanY = 13;
	power_pill = 0;
	food_pellats = 74;

	// Setting outer walls in game state, 1 is for walls
	for (int i=0;i<19;i++){
		complete_game_state[i][0] = complete_game_state[i][20] = complete_game_state[0][i] = complete_game_state[18][i] = 1;
	}
	complete_game_state[0][19] = complete_game_state[0][20] = complete_game_state[18][19] = complete_game_state[18][20] = 1;
	//complete_game_state[]

	//TODO add all the walls
	// Walls of the game, line by line (literally), each line is a bloack
	complete_game_state[2][2] = complete_game_state[3][2] = 1;
	complete_game_state[5][2] = complete_game_state[6][2] = complete_game_state[7][2] = 1;
	complete_game_state[9][2] = 1;
	complete_game_state[11][2] = complete_game_state[12][2] = complete_game_state[13][2] = 1;
	complete_game_state[15][2] = complete_game_state[16][2] = 1;

	complete_game_state[2][4] = complete_game_state[3][4] = 1;
	complete_game_state[5][4] = 1;
	complete_game_state[7][4] = complete_game_state[8][4] = complete_game_state[9][4] = complete_game_state[10][4] = complete_game_state[11][4] = 1;
	complete_game_state[13][4] = 1;
	complete_game_state[15][4] = complete_game_state[16][4] = 1;

	complete_game_state[5][5] = 1;
	complete_game_state[9][5] = 1;
	complete_game_state[13][5] = 1;

	complete_game_state[1][6] = complete_game_state[2][6] = complete_game_state[3][6] = 1;
	complete_game_state[5][6] = complete_game_state[6][6] = complete_game_state[7][6] = 1;
	complete_game_state[9][6] = 1;
	complete_game_state[11][6] = complete_game_state[12][6] = complete_game_state[13][6] = 1;
	complete_game_state[15][6] = complete_game_state[16][6] = complete_game_state[17][6] = 1;

	complete_game_state[1][7] = complete_game_state[2][7] = complete_game_state[3][7] = 1;
	complete_game_state[5][7] = 1;
	complete_game_state[13][7] = 1;
	complete_game_state[15][7] = complete_game_state[16][7] = complete_game_state[17][7] = 1;

	complete_game_state[1][8] = complete_game_state[2][8] = complete_game_state[3][8] = 1;
	complete_game_state[5][8] = 1;
	complete_game_state[7][8] = 1;
	complete_game_state[11][8] = 1;
	complete_game_state[13][8] = 1;
	complete_game_state[15][8] = complete_game_state[16][8] = complete_game_state[17][8] = 1;

	// Removing walls in sides
	complete_game_state[0][9] = 0;
	complete_game_state[5][9] = 1;
	complete_game_state[7][9] = 1;
	complete_game_state[11][9] = 1;
	complete_game_state[13][9] = 1;
	complete_game_state[18][9] = 0;

	complete_game_state[1][10] = complete_game_state[2][8] = complete_game_state[3][8] = 1;
	complete_game_state[3][10] = 1;
	complete_game_state[5][10] = 1;
	complete_game_state[7][10] = 1;
	complete_game_state[8][10] = 1;
	complete_game_state[9][10] = 1;
	complete_game_state[10][10] = 1;
	complete_game_state[11][10] = 1;
	complete_game_state[13][10] = 1;
	complete_game_state[15][10] = complete_game_state[16][8] = complete_game_state[17][8] = 1;

	complete_game_state[1][11] = complete_game_state[2][8] = complete_game_state[3][8] = 1;
	complete_game_state[3][11] = 1;
	complete_game_state[5][11] = 1;
	complete_game_state[13][11] = 1;
	complete_game_state[15][11] = complete_game_state[16][8] = complete_game_state[17][8] = 1;

	complete_game_state[1][12] = complete_game_state[2][8] = complete_game_state[3][8] = 1;
	complete_game_state[3][12] = 1;
	complete_game_state[5][12] = 1;
	complete_game_state[7][12] = 1;
	complete_game_state[8][12] = 1;
	complete_game_state[9][12] = 1;
	complete_game_state[10][12] = 1;
	complete_game_state[11][12] = 1;
	complete_game_state[13][12] = 1;
	complete_game_state[15][12] = complete_game_state[16][8] = complete_game_state[17][8] = 1;

	complete_game_state[2][14] = complete_game_state[3][14] = 1;
	complete_game_state[5][14] = complete_game_state[6][14] = complete_game_state[7][14] = 1;
	complete_game_state[9][14] = 1;
	complete_game_state[11][14] = complete_game_state[12][14] = complete_game_state[13][14] = 1;
	complete_game_state[15][14] = complete_game_state[16][14] = 1;

	complete_game_state[3][15] = 1;
	complete_game_state[15][15] = 1;

	complete_game_state[1][16] = 1;
	complete_game_state[3][16] = 1;
	complete_game_state[5][16] = 1;
	complete_game_state[7][16] = complete_game_state[8][16] = complete_game_state[9][16] = complete_game_state[10][16] = complete_game_state[11][16] = 1;
	complete_game_state[13][16] = 1;
	complete_game_state[15][16] = 1;
	complete_game_state[17][16] = 1;

	complete_game_state[5][17] = 1;
	complete_game_state[9][17] = 1;
	complete_game_state[13][17] = 1;

	complete_game_state[2][18] = complete_game_state[3][18] = complete_game_state[4][18] = complete_game_state[5][18] = complete_game_state[6][18] = complete_game_state[7][18] = 1;
	complete_game_state[9][18] = 1;
	complete_game_state[11][18] = complete_game_state[12][18] = complete_game_state[13][18] = complete_game_state[14][18] = complete_game_state[15][18] = complete_game_state[16][18] = 1;


	// 2 is food pellats for complete game state
	// Randomised pellet positions
	for (int i=0;i<19;i++){
		for (int j=0;j<21;j++) {
			if (complete_game_state[i][j] == 0) {
				complete_game_state[i][j] = (rand01() < 0.5) ? 2 : 0;
			}
		}
	}


	/*
	Static pellet positiosn
	complete_game_state[3][1] = complete_game_state[4][1] = complete_game_state[2][1] = complete_game_state[13][1] = complete_game_state[14][1] = complete_game_state[15][1] = complete_game_state[16][1] = complete_game_state[12][1] = 2;
	complete_game_state[1][2] = complete_game_state[8][2] = complete_game_state[17][2] = 2;
	complete_game_state[7][3] = complete_game_state[8][3] = complete_game_state[9][3] = complete_game_state[10][3] = complete_game_state[12][3] = complete_game_state[15][3] = 2;
	complete_game_state[1][4] = complete_game_state[12][4] = 2;
	complete_game_state[1][5] = complete_game_state[2][5] = complete_game_state[4][5] = complete_game_state[6][5] = complete_game_state[7][5] = 2;
	complete_game_state[4][6] = complete_game_state[14][6] = 2;
	complete_game_state[14][7] = 2;
	complete_game_state[4][10] = 2;
	complete_game_state[4][11] = 2;
	complete_game_state[12][12] = 2;
	complete_game_state[4][13] = complete_game_state[5][13] = complete_game_state[7][13] = complete_game_state[11][13] = complete_game_state[13][13] = complete_game_state[14][13] = complete_game_state[15][13] = complete_game_state[16][13] = complete_game_state[17][13] = 2;
	complete_game_state[8][14] = complete_game_state[14][14] = 2;
	complete_game_state[2][15] = complete_game_state[4][15] = complete_game_state[11][15] = complete_game_state[13][15] = complete_game_state[14][15] = 2;
	complete_game_state[2][16] = complete_game_state[4][16] = complete_game_state[14][16] = 2;
	complete_game_state[3][17] = complete_game_state[7][17] = complete_game_state[8][17] = complete_game_state[10][17] = complete_game_state[11][17] = complete_game_state[12][17] = 2;
	complete_game_state[10][18] = 2;
	complete_game_state[1][19] = complete_game_state[2][19] = complete_game_state[6][19] = complete_game_state[7][19] = complete_game_state[8][19] = complete_game_state[11][19] = complete_game_state[15][19] = complete_game_state[16][19] = complete_game_state[17][19] = 2;
	*/


	// 6 is for power pill
	complete_game_state[1][3] = complete_game_state[17][3] = complete_game_state[1][15] = complete_game_state[17][15] = 6;


	// Printing the screen
	for (int j = 0; j < 21; ++j)
	        {
		for (int i = 0; i < 19; ++i)
	    {
        	if ((pacmanX == i) && (pacmanY == j)) {
        		// Pacman
        		std::cout << "P" << ' ';
        	} else if (complete_game_state[i][j] == 0) {
        		// Empty
        		std::cout << " " << ' ';
        	} else if (complete_game_state[i][j] == 1) {
        		// Wall
        		std::cout << "#" << ' ';
        	} else if (complete_game_state[i][j] == 4) {
        		// Ghost
        		std::cout << "G" << ' ';
        	} else if (complete_game_state[i][j] == 2) {
        		// food pellet
        		std::cout << "o" << ' ';
        	} else if (complete_game_state[i][j] == 6) {
        		// power pill
        		std::cout << "O" << ' ';
        	} else {
        		std::cout << "?" << ' ';
        	}
            //std::cout << complete_game_state[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}


Pacman::Pacman(options_t &options) {
	//Also stuff
	
	reset_game();

	// Set up the initial observation
	// wall_obs = (left_wall * pow(2,0)) + (right_wall * pow(2,1)) + (up_wall * pow(2,2)) + (down_wall * pow(2,3));
	// ghost_obs = (left_ghost * pow(2,4)) + (right_ghost * pow(2,5)) + (up_ghost * pow(2,6)) + (down_ghost * pow(2,7));
	// smell_obs = (pellat_within_2 * pow(2,8)) + (pellat_within_3 * pow(2,9)) + (pellat_within_4 * pow(2,10));
	// pellat_obs = (left_food * pow(2,11)) + (right_food * pow(2,12)) + (up_food * pow(2,13)) + (down_food * pow(2,14));
	// power_obs = (power_pill * pow(2,15));

	wall_obs = (0 * pow(2,0)) + (0 * pow(2,1)) + (1 * pow(2,2)) + (1 * pow(2,3));
	ghost_obs = (0 * pow(2,4)) + (0 * pow(2,5)) + (0 * pow(2,6)) + (0 * pow(2,7));
	smell_obs = (1 * pow(2,8)) + (0 * pow(2,9)) + (1 * pow(2,10));
	pellat_obs = (1 * pow(2,11)) + (1 * pow(2,12)) + (0 * pow(2,13)) + (0 * pow(2,14));
	power_obs = (0 * pow(2,15));

	// Combine all the observations
	m_observation = wall_obs + ghost_obs + smell_obs + pellat_obs + power_obs;
}

Pacman::Ghost Pacman::ghost_movement(Ghost ghost) {
	// stuff
	// check if distance is less then or equal to 5
	ghost.oldx = ghost.x;
	ghost.oldy = ghost.y;

	// Pursue pacman
	if (ghost.manhattan_distance <= 5) {
		//pursue pacman
		// TODO turn into a function
		// Check which quadrant pacman is in, TopLeft, etc
		if (ghost.x > pacmanX)  {
			if (ghost.y > pacmanY) {
				// ghost moves UP or left
				if (complete_game_state[ghost.x][ghost.y - 1] != 1) {
					// no wall above
					// move up
					ghost.action = 4;
				} else if (complete_game_state[ghost.x - 1][ghost.y] != 1) {
					// no wall to left
					ghost.action = 2;
				} else {
					// if the ghost cannot go towards pacman, it will move in random other direction
					if (rand01() < 0.5) {
						ghost.action = 1;
					} else {
						ghost.action = 3;
					}
				}
			} else {
				// ghost moves up or right
				if (complete_game_state[ghost.x][ghost.y - 1] != 1) {
					// no wall above
					// move up
					ghost.action = 4;
				} else if (complete_game_state[ghost.x + 1][ghost.y] != 1) {
					// no wall to right
					ghost.action = 1;
				} else {
					// if the ghost cannot go towards pacman, it will move in random other direction
					if (rand01() < 0.5) {
						ghost.action = 2;
					} else {
						ghost.action = 3;
					}
				}
			}
		} else {
			if (ghost.y > pacmanY) {
				// ghost moves down or left
				if (complete_game_state[ghost.x][ghost.y + 1] != 1) {
					// no wall below
					// move down
					ghost.action = 3;
				} else if (complete_game_state[ghost.x - 1][ghost.y] != 1) {
					// no wall to left
					ghost.action = 2;
				} else {
					// if the ghost cannot go towards pacman, it will move in random other direction
					if (rand01() < 0.5) {
						ghost.action = 1;
					} else {
						ghost.action = 4;
					}
				}
			} else {
				// ghost moves down or right
				if (complete_game_state[ghost.x][ghost.y + 1] != 1) {
					// no wall below
					// move down
					ghost.action = 3;
				} else if (complete_game_state[ghost.x + 1][ghost.y] != 1) {
					// no wall to right
					ghost.action = 1;
				} else {
					// if the ghost cannot go towards pacman, it will move in random other direction
					if (rand01() < 0.5) {
						ghost.action = 2;
					} else {
						ghost.action = 4;
					}
				}
			}
		}
		
	} else {
		//random move
		random_number = rand01();
		if (random_number < 0.25) {
			ghost.action = 1;
		} else if (random_number < 0.5) {
			ghost.action = 2;
		} else if (random_number < 0.75) {
			ghost.action = 3;
		} else {
			ghost.action = 4;
		}
	}
	// ghost.action = (random_number < 0.25) ? 1 : ((random_number < 0.5) ? 2 : ((random_number < 0.75) ? 3 : 4));

	// change the square the ghost moved out of

	std::cout << "Ghost " << " X: " << ghost.x << " Y: " << ghost.y << std::endl;

	switch(ghost.action) {
		case 1:
			if (complete_game_state[ghost.x+1][ghost.y] != 1) {
				ghost.x += 1;
			}
			break;
		case 2:
			if (complete_game_state[ghost.x-1][ghost.y] != 1) {
				ghost.x -= 1;
			}
			break;
		case 3:
			if (complete_game_state[ghost.x][ghost.y+1] != 1) {
				ghost.y += 1;
			}
			break;
		case 4:
			if (complete_game_state[ghost.x][ghost.y-1] != 1) {
				ghost.y -= 1;
			}
			break;

	}

	std::cout << "Ghost " << " X: " << ghost.x << " Y: " << ghost.y << std::endl;

	// check and change the square the ghost is moving into
	if (complete_game_state[ghost.x][ghost.y] == 2) {
		// food and ghost
		complete_game_state[ghost.x][ghost.y] = 8;
	} else if (complete_game_state[ghost.x][ghost.y] == 6) {
		// power pills and ghost
		complete_game_state[ghost.x][ghost.y] = 10;
	} else if (complete_game_state[ghost.x][ghost.y] == 0) {
		// blank space
		complete_game_state[ghost.x][ghost.y] = 4;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 4) {
		// another ghost
		complete_game_state[ghost.oldx][ghost.oldy] = 12;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 12) {
		// 2 other ghosts
		complete_game_state[ghost.oldx][ghost.oldy] = 14;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 14) {
		// 3 other ghosts
		complete_game_state[ghost.oldx][ghost.oldy] = 16;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 18) {
		// 2 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 8;
	} else if (complete_game_state[ghost.x][ghost.y] == 20) {
		// 3 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 18;
	} else if (complete_game_state[ghost.x][ghost.y] == 22) {
		// 4 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 20;
	}


	if (complete_game_state[ghost.oldx][ghost.oldy] == 8 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// Food pellets
		complete_game_state[ghost.oldx][ghost.oldy] = 2;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 10 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// power pill
		complete_game_state[ghost.oldx][ghost.oldy] = 6;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 4 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// blank space
		complete_game_state[ghost.oldx][ghost.oldy] = 0;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 12 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// 1 other ghost
		complete_game_state[ghost.oldx][ghost.oldy] = 4;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 14 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// 2 other ghsots
		complete_game_state[ghost.oldx][ghost.oldy] = 12;
	} else if (complete_game_state[ghost.oldx][ghost.oldy] == 16 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy) ) {
		// 3 other ghosts
		complete_game_state[ghost.oldx][ghost.oldy] = 14;
	} else if (complete_game_state[ghost.x][ghost.y] == 8 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy)) {
		// 2 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 18;
	} else if (complete_game_state[ghost.x][ghost.y] == 18 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy)) {
		// 3 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 20;
	} else if (complete_game_state[ghost.x][ghost.y] == 20 && (ghost.x != ghost.oldx || ghost.y != ghost.oldy)) {
		// 4 ghosts and food pellet
		complete_game_state[ghost.x][ghost.y] = 22;
	}

	
	
	return ghost;
}


void Pacman::performAction(action_t action) {
	// Rewards normalised for -70

	if (power_pill_time < 1 ) {
		power_pill_time = 0;
		power_pill = 0;
	} else {
		power_pill_time -= 1;
	}
	//ghost movement
	//check manhatten distance
	// action 1 X += 1;
	// action 2 X -= 1;
	// action 3 Y += 1;
	// action 4 Y -= 1;
	ghost1 = ghost_movement(ghost1);
	ghost2 = ghost_movement(ghost2);
	ghost3 = ghost_movement(ghost3);
	ghost4 = ghost_movement(ghost4);


	// and pacman movement
	if (action == 0){
		new_position = complete_game_state[pacmanX+1][pacmanY];
	} else if (action == 1){
		new_position = complete_game_state[pacmanX-1][pacmanY];
	} else if (action == 2){
		new_position = complete_game_state[pacmanX][pacmanY+1];
	} else {
		new_position = complete_game_state[pacmanX][pacmanY-1];
	}
	switch(new_position) {
		case 0:
			//the new position is a blank space, pacman moves into it
			if (action == 0){
				pacmanX += 1;
			} else if (action == 1){
				pacmanX -= 1;
			} else if (action == 2){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			m_reward = 59;
			break;
		case 1:
			//The new position is a wall, and pacman can't move into it
			//pacmanX = pacmanX;
			//pacmanY = pacmanY;
			std::cout << "Cannot move into walls" << std::endl;
			m_reward = 50;
			break;
		case 2:
			//The new position is a food pellat
			if (action == 0){
				pacmanX += 1;
			} else if (action == 1){
				pacmanX -= 1;
			} else if (action == 2){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			m_reward = 70;
			food_pellats -= 1;
			complete_game_state[pacmanX][pacmanY] = 0;
			if (food_pellats == 0) {
				// if no food pellats left, pacman wins
				m_reward = 170;
				// End the game somehow?
				// episode ends
				reset_game();
			}
			break;
		case 6:
			// If pacman eats a powerpill, it lasts for ?10 seconds
			power_pill = 1;
			power_pill_time = 10;
			if (action == 0){
				pacmanX += 1;
			} else if (action == 1){
				pacmanX -= 1;
			} else if (action == 2){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			complete_game_state[pacmanX][pacmanY] = 0;
			break;
		case 8:
			// food pellat and ghost
			if (action == 0){
				pacmanX += 1;
			} else if (action == 1){
				pacmanX -= 1;
			} else if (action == 2){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			m_reward = 70;
			food_pellats -= 1;
			complete_game_state[pacmanX][pacmanY] = 4;
			if (food_pellats == 0) {
				// if no food pellats left, pacman wins
				m_reward = 170;
				// End the game somehow?
				// episode ends
				reset_game();
			}
			break;
		case 10:
			// power_pill and ghost
			power_pill = 1;
			power_pill_time = 10;
			if (action == 0){
				pacmanX += 1;
			} else if (action == 1){
				pacmanX -= 1;
			} else if (action == 2){
				pacmanY += 1;
			} else {
				pacmanY -= 1;
			}
			complete_game_state[pacmanX][pacmanY] = 4;
			break;
	}


	
	//check if caught by ghosts

	// Initial ghost positions
	// ghost1 = {8, 9, 5, 1};
	// ghost2 = {8, 10, 5, 1};
	// ghost3 = {9, 9, 6, 1};
	// ghost4 = {9, 10, 4, 1};

	// Catching pacman / cacthing ghosts
	if ((ghost1.x == pacmanX && ghost1.y == pacmanY) || (ghost2.x == pacmanX && ghost2.y == pacmanY) || (ghost3.x == pacmanX && ghost3.y == pacmanY) || (ghost4.x == pacmanX && ghost4.y == pacmanY)) {
		if (power_pill) {
			// check which ghosts were hit
			if (ghost1.x == pacmanX && ghost1.y == pacmanY) {
				complete_game_state[ghost1.x][ghost1.y] = 0;
				ghost1.x = 9;
				ghost1.y = 8;
				complete_game_state[ghost1.x][ghost1.y] = 4;
			}
			if (ghost2.x == pacmanX && ghost2.y == pacmanY) {
				complete_game_state[ghost2.x][ghost2.y] = 0;
				ghost2.x = 10;
				ghost2.y = 8;
				complete_game_state[ghost2.x][ghost2.y] = 4;
			}
			if (ghost3.x == pacmanX && ghost3.y == pacmanY) {
				complete_game_state[ghost3.x][ghost3.y] = 0;
				ghost3.x = 9;
				ghost3.y = 9;
				complete_game_state[ghost3.x][ghost3.y] = 4;
			}
			if (ghost4.x == pacmanX && ghost4.y == pacmanY) {
				complete_game_state[ghost4.x][ghost4.y] = 0;
				ghost4.x = 10;
				ghost4.y = 9;
				complete_game_state[ghost4.x][ghost4.y] = 4;
			}
		} else {
			m_reward = 10;
			// End the game somehow?
			//game ends
			reset_game();
		}
	} else if ((ghost1.manhattan_distance == 1 && (ghost1.action + action == 3 || ghost1.action + action == 7)  && (abs(ghost1.x -pacmanX) + abs(ghost1.y -pacmanY) == 1)) || (ghost2.manhattan_distance == 1 && (ghost2.action + action == 3 || ghost2.action + action == 7)  && (abs(ghost2.x -pacmanX) + abs(ghost2.y -pacmanY) == 1)) || (ghost3.manhattan_distance == 1 && (ghost3.action + action == 3 || ghost3.action + action == 7)  && (abs(ghost3.x -pacmanX) + abs(ghost3.y -pacmanY) == 1)) || (ghost4.manhattan_distance == 1 && (ghost4.action + action == 3 || ghost4.action + action == 7) && (abs(ghost4.x -pacmanX) + abs(ghost4.y -pacmanY) == 1))) {
		//check if moved through any of the ghosts
		// by checking if old and current manhattan_distance is 1 and both pacman and ghost both moved in opposite directions in the same axis
		if (power_pill) {
			// check which ghosts were hit
			if (ghost1.manhattan_distance == 1 && (ghost1.action + action == 3 || ghost1.action + action == 7)  && (abs(ghost1.x -pacmanX) + abs(ghost1.y -pacmanY) == 1)) {
				complete_game_state[ghost1.x][ghost1.y] = 0;
				ghost1.x = 9;
				ghost1.y = 8;
				complete_game_state[ghost1.x][ghost1.y] = 4;
			}
			if (ghost2.manhattan_distance == 1 && (ghost2.action + action == 3 || ghost2.action + action == 7)  && (abs(ghost2.x -pacmanX) + abs(ghost2.y -pacmanY) == 1)) {
				complete_game_state[ghost2.x][ghost2.y] = 0;
				ghost2.x = 10;
				ghost2.y = 8;
				complete_game_state[ghost2.x][ghost2.y] = 4;
			}
			if (ghost3.manhattan_distance == 1 && (ghost3.action + action == 3 || ghost3.action + action == 7)  && (abs(ghost3.x -pacmanX) + abs(ghost3.y -pacmanY) == 1)) {
				complete_game_state[ghost3.x][ghost3.y] = 0;
				ghost3.x = 9;
				ghost3.y = 9;
				complete_game_state[ghost3.x][ghost3.y] = 4;
			}
			if (ghost4.manhattan_distance == 1 && (ghost4.action + action == 3 || ghost4.action + action == 7)  && (abs(ghost4.x -pacmanX) + abs(ghost4.y -pacmanY) == 1)) {
				complete_game_state[ghost4.x][ghost4.y] = 0;
				ghost4.x = 10;
				ghost4.y = 9;
				complete_game_state[ghost4.x][ghost4.y] = 4;
			}
		} else {
			m_reward = 10;
			// End the game somehow?
			//game ends
			reset_game();
		}
	}
	// calculate mantahhan distances
	ghost1.manhattan_distance = abs(ghost1.x -pacmanX) + abs(ghost1.y -pacmanY);
	ghost2.manhattan_distance = abs(ghost2.x -pacmanX) + abs(ghost2.y -pacmanY);
	ghost3.manhattan_distance = abs(ghost3.x -pacmanX) + abs(ghost3.y -pacmanY);
	ghost4.manhattan_distance = abs(ghost4.x -pacmanX) + abs(ghost4.y -pacmanY);


	//Resolve Observations

	// 4-bit describing neighbouring walls
	left_wall = (complete_game_state[pacmanX-1][pacmanY] == 1) ? 1 : 0;
	right_wall = (complete_game_state[pacmanX+1][pacmanY] == 1) ? 1 : 0;
	up_wall = (complete_game_state[pacmanX][pacmanY-1] == 1) ? 1 : 0;
	down_wall = (complete_game_state[pacmanX][pacmanY+1] == 1) ? 1 : 0;


	// 4-bit describing whether or not ghost is visible

	// Check left
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < pacmanX+1) && (!left_food) ) {
		if (complete_game_state[pacmanX - distance][pacmanY] == 1) {
			wall = 1;
			left_ghost = 0;
		} else if ((complete_game_state[pacmanX - distance][pacmanY] == 4) || (complete_game_state[pacmanX - distance][pacmanY] == 8)  || (complete_game_state[pacmanX - distance][pacmanY] == 10)) {
			left_ghost = 1;
		}
		distance++;
	}

	// Check right
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < 22) && (!right_food) ) {
		if (complete_game_state[pacmanX + distance][pacmanY] == 1) {
			wall = 1;
			right_ghost = 0;
		} else if ((complete_game_state[pacmanX + distance][pacmanY] == 4) || (complete_game_state[pacmanX + distance][pacmanY] == 8)  || (complete_game_state[pacmanX + distance][pacmanY] == 10)) {
			right_ghost = 1;
		}
		distance++;
	}

	// Check up
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < pacmanY + 1) && (!up_food) ) {
		if (complete_game_state[pacmanX][pacmanY - distance] == 1) {
			wall = 1;
			up_ghost = 0;
		} else if ((complete_game_state[pacmanX][pacmanY - distance] == 4) || (complete_game_state[pacmanX][pacmanY - distance] == 8)  || (complete_game_state[pacmanX][pacmanY - distance] == 10)) {
			up_ghost = 1;
		}
		distance++;
	}

	// Check down
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < 24) && (!down_food) ) {
		if (complete_game_state[pacmanX][pacmanY + distance] == 1) {
			wall = 1;
			down_ghost = 0;
		} else if ((complete_game_state[pacmanX][pacmanY + distance] == 4) || (complete_game_state[pacmanX][pacmanY + distance] == 8)  || (complete_game_state[pacmanX][pacmanY + distance] == 10)) {
			down_ghost = 1;
		}
		distance++;
	}



	// 3-bit observation on whether or not a pellat is within 2,3,4 manhattan distance from pacman

	// TODO implement as a tree style search under a while loop, only need one to be true
	// Pellat is within 2
	pellat_within_2 = (complete_game_state[pacmanX+2][pacmanY] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX-2][pacmanY] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX][pacmanY+2] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX][pacmanY-2] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX+1][pacmanY+1] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX+1][pacmanY-1] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX-1][pacmanY+1] == 2 || pellat_within_2) ? 1 : 0;
	pellat_within_2 = (complete_game_state[pacmanX-1][pacmanY-1] == 2 || pellat_within_2) ? 1 : 0;

	// Pellat is within 3
	pellat_within_3 = (complete_game_state[pacmanX+3][pacmanY] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX-3][pacmanY] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX][pacmanY+3] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX][pacmanY-3] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX+2][pacmanY+1] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX+2][pacmanY-1] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX-2][pacmanY+1] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX-2][pacmanY-1] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX+1][pacmanY+2] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX+1][pacmanY-2] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX-1][pacmanY+2] == 2 || pellat_within_3) ? 1 : 0;
	pellat_within_3 = (complete_game_state[pacmanX-1][pacmanY-2] == 2 || pellat_within_3) ? 1 : 0;

	// Pellat is within 4
	pellat_within_4 = (complete_game_state[pacmanX+4][pacmanY] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-4][pacmanY] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX][pacmanY+4] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX][pacmanY-4] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+3][pacmanY+1] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+3][pacmanY-1] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-3][pacmanY+1] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-3][pacmanY-1] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+1][pacmanY+3] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+1][pacmanY-3] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-1][pacmanY+3] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-1][pacmanY-3] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+2][pacmanY+2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+2][pacmanY-2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-2][pacmanY+2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-2][pacmanY-2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+2][pacmanY+2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX+2][pacmanY-2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-2][pacmanY+2] == 2 || pellat_within_4) ? 1 : 0;
	pellat_within_4 = (complete_game_state[pacmanX-2][pacmanY-2] == 2 || pellat_within_4) ? 1 : 0;

	// 4-bit describing whether of not food is in line of sight

	// Assuming you can see through ghosts

	// Check left
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < pacmanX+1) && (!left_food) ) {
		if (complete_game_state[pacmanX - distance][pacmanY] == 1) {
			wall = 1;
			left_food = 0;
		} else if (complete_game_state[pacmanX - distance][pacmanY] == 2) {
			left_food = 1;
		}
		distance++;
	}

	// Check right
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < 22) && (!right_food) ) {
		if (complete_game_state[pacmanX + distance][pacmanY] == 1) {
			wall = 1;
			right_food = 0;
		} else if (complete_game_state[pacmanX + distance][pacmanY] == 2) {
			right_food = 1;
		}
		distance++;
	}

	// Check up
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < pacmanY + 1) && (!up_food) ) {
		if (complete_game_state[pacmanX][pacmanY - distance] == 1) {
			wall = 1;
			up_food = 0;
		} else if (complete_game_state[pacmanX][pacmanY - distance] == 2) {
			up_food = 1;
		}
		distance++;
	}

	// Check down
	wall = 0;
	distance = 1;
	while ( (!wall) && (distance < 24) && (!down_food) ) {
		if (complete_game_state[pacmanX][pacmanY + distance] == 1) {
			wall = 1;
			down_food = 0;
		} else if (complete_game_state[pacmanX][pacmanY + distance] == 2) {
			down_food = 1;
		}
		distance++;
	}

	// 1-bit describing whether or not pacman is under effects of power pill

	// Combine each of the observations
	wall_obs = (left_wall * pow(2,0)) + (right_wall * pow(2,1)) + (up_wall * pow(2,2)) + (down_wall * pow(2,3));
	ghost_obs = (left_ghost * pow(2,4)) + (right_ghost * pow(2,5)) + (up_ghost * pow(2,6)) + (down_ghost * pow(2,7));
	smell_obs = (pellat_within_2 * pow(2,8)) + (pellat_within_3 * pow(2,9)) + (pellat_within_4 * pow(2,10));
	pellat_obs = (left_food * pow(2,11)) + (right_food * pow(2,12)) + (up_food * pow(2,13)) + (down_food * pow(2,14));
	power_obs = (power_pill * pow(2,15));

	// Combine all the observations
	m_observation = wall_obs + ghost_obs + smell_obs + pellat_obs + power_obs;

	// Remove extra ghosts
	for (int j = 0; j < 21; ++j)
	        {
		for (int i = 0; i < 19; ++i)
	    {
	    	if (complete_game_state[i][j] == 4 || complete_game_state[i][j] == 12 || complete_game_state[i][j] == 14 || complete_game_state[i][j] == 16 ) {
	    		if (!((ghost1.x == i && ghost1.y == j) || (ghost2.x == i && ghost2.y == j) || (ghost3.x == i && ghost3.y == j) || (ghost4.x == i && ghost4.y == j))) {
	    			complete_game_state[i][j] = 0;
	    		}
	    	} else if (complete_game_state[i][j] == 18 || complete_game_state[i][j] == 20 || complete_game_state[i][j] == 22 || complete_game_state[i][j] == 8 ) {
	    		if (!((ghost1.x == i && ghost1.y == j) || (ghost2.x == i && ghost2.y == j) || (ghost3.x == i && ghost3.y == j) || (ghost4.x == i && ghost4.y == j))) {
	    			complete_game_state[i][j] = 2;
	    		}
	    	}
	    }
	   }

	// Printing the screen
	for (int j = 0; j < 21; ++j)
	        {
		for (int i = 0; i < 19; ++i)
	    {
        	if ((pacmanX == i) && (pacmanY == j)) {
        		// Pacman
        		std::cout << "P" << ' ';
        	} else if (complete_game_state[i][j] == 0) {
        		// Empty
        		std::cout << " " << ' ';
        	} else if (complete_game_state[i][j] == 1) {
        		// Wall
        		std::cout << "#" << ' ';
        	} else if (complete_game_state[i][j] == 4 || complete_game_state[i][j] == 12 || complete_game_state[i][j] == 14 || complete_game_state[i][j] == 16 || complete_game_state[i][j] == 8 || complete_game_state[i][j] == 18 || complete_game_state[i][j] == 20 || complete_game_state[i][j] == 22) {
        		// Ghost
        		std::cout << "G" << ' ';
        	} else if (complete_game_state[i][j] == 2) {
        		// food pellet
        		std::cout << "o" << ' ';
        	} else if (complete_game_state[i][j] == 6) {
        		// power pill
        		std::cout << "O" << ' ';
        	} else {
        		std::cout << complete_game_state[i][j] << ' ';
        	}
            //std::cout << complete_game_state[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "Pacman X and Y: " << pacmanX << " , " << pacmanY << std::endl;
    std::cout << "Ghost1 X and Y: " << ghost1.x << " , " << ghost1.y << std::endl;
    std::cout << "Ghost2 X and Y: " << ghost2.x << " , " << ghost2.y << std::endl;
    std::cout << "Ghost3 X and Y: " << ghost3.x << " , " << ghost3.y << std::endl;
    std::cout << "Ghost4 X and Y: " << ghost4.x << " , " << ghost4.y << std::endl;

}


void TicTacToe::reset_game() {
	// reset the board
	state = 0;
	m_observation = state;

	// The win conditions of agent and opponent
	win_cond = 0;
	opponent_win_cond = 0;

	//open_squares = (1,2,3,4,5,6,7,8,9);

	// The square which have yet to be used
	open_squares = {0,1,2,3,4,5,6,7,8};

	// if the game is finished
	game_finished = 0;

	// Set up board, 0 for empty, -1 for O and 1 for X
	for (int i = 0; i < 3 ; i++) {
		for (int j = 0 ; j < 3 ; j++) {
			board[i][j] = 0;
		}
	}
}


TicTacToe::TicTacToe(options_t &options) {
	// Initial state reward and observation, state is integer = observation
	state = 0;
	m_reward = 0;
	m_observation = state;

	// The win conditions of agent and opponent
	win_cond = 0;
	opponent_win_cond = 0;

	//open_squares = (1,2,3,4,5,6,7,8,9);

	// The square which have yet to be used
	open_squares = {0,1,2,3,4,5,6,7,8};

	// if the game is finished
	game_finished = 0;

	// Set up board, 0 for empty, -1 for O and 1 for X
	for (int i = 0; i < 3 ; i++) {
		for (int j = 0 ; j < 3 ; j++) {
			board[i][j] = 0;
		}
	}
}


void TicTacToe::performAction(action_t action) {
	//Agent move is added to state
	//If he wins game ends, if illegal lose points
	// All positive rewards, normalised at 3

	// to find the nth number base 2 in the state, b = state/pow(2,n-1) % 4


	// std::cout << "Action / 3: " << action / 3 <<  " Action mod 3: " << action % 3 << std::endl;
	// std::cout << "Board[above][above]: " << board[action / 3][action % 3] << std::endl;
	// std::cout << "Open tile size: " << open_squares.size() << std::endl;

	if ( board[action / 3][action % 3] != 0) {
		// (fmod((state/pow(4, action)),2) == 1) || (fmod((state/(pow(4, action)*2) ),2) == 1) 
		// If the agent performs an illegal move
		m_reward = 0;
		// std::cout << "Cannot place tile here " << std::endl;
	} else {
		// Agent move is added to state
		state += pow(4,action);
		board[(action / 3)][(action % 3)] = 1;
		// std::cout << " Placed tile at above" << std::endl;
		// std::cout << "Board: " << board[(action / 3)][(action % 3)] << std::endl;
		// Remove the square from open squares
		open_squares.erase(std::remove(open_squares.begin(), open_squares.end(), action), open_squares.end());
		// Reward for an action
		m_reward = 3;
		// Check is game is finished
		win_cond = ( (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][2] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][2] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][2] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[2][0] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[2][1] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[2][2] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] == 1) || win_cond == 1) ? 1 : 0;
		win_cond = ( (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[2][0] == 1) || win_cond == 1) ? 1 : 0;
	}
	if (win_cond == 1) {
		// If game is finished, then agent has won, and get reward of 5
		game_finished = 1;
		reset_game();
		m_reward = 5;
		// std::cout << "Game won!" << std::endl;

	// check if board fills up
	} else if (open_squares.size() == 0) {
		// game is a draw
		game_finished = 1;
		reset_game();
		m_reward = 4;
		// std::cout << "Game draw!" << std::endl;
		// std::cout << "Open squares: " << open_squares.size() << std::endl;
	} else {
		// find how many moves are possible, then make random move
		// number_of_open_sqaures = open_squares.size();
		

		/*
		// Alternative randomisation

		#include <random>
		std::random_device random_device;
		std::mt19937 engine{random_device()};
		std::uniform_int_distribution<int> dist(0, open_squares.size() - 1);
		
		int random_choice = open_squares[dist(engine)];

		*/

		std::random_shuffle ( open_squares.begin(), open_squares.end() );
		random_choice = open_squares[0];

		state += pow(4,random_choice)*2;


		board[(random_choice / 3)][(random_choice % 3)] = -1;
		// std::cout << "Opponent move: " << (random_choice / 3) << " , " << (random_choice % 3) << std::endl;

		open_squares.erase(std::remove(open_squares.begin(), open_squares.end(), random_choice), open_squares.end());

		// could implement as a tree style search under a while loop, only need one to be true
		opponent_win_cond = ( (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][2] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][2] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][2] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[2][0] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[2][1] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[2][2] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] == (-1)) || opponent_win_cond == 1) ? 1 : 0;
		opponent_win_cond = ( (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[2][0] == (-1)) || opponent_win_cond == 1) ? 1 : 0;

		if (opponent_win_cond == 1) {
			game_finished = 1;
			reset_game();
			m_reward = 1;
		} else if (open_squares.size() == 0) {
			game_finished = 1;
			reset_game();
			m_reward = 4;
		}
		
	}
	m_observation = state;

	// Printing the screen

	for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << board[i][j] << ' ';
            //std::cout << (char)127 << ' ';
        }
        std::cout << std::endl;
    }

	//Random Opponent makes moves
	//if he wins game ends, if not give observations

}


BiasedRockPaperSciessor::BiasedRockPaperSciessor(options_t &options) {
	// Set up the initial observation and reward
	m_reward = 0;
	m_observation = 0;
	opponent_won_last_round = 0;
	
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
		m_reward = 1;
		m_observation = opponent_action;
		opponent_won_last_round = 0;
	} else if ((opponent_action == 0 && action == 1) || (opponent_action == 1 && action == 2) || (opponent_action == 2 && action == 0)) {
		m_reward = 2;
		m_observation = opponent_action;
		opponent_won_last_round = 0;
	} else {
		m_reward = 0;
		m_observation = opponent_action;
		opponent_won_last_round = 1;
	}
}


ExtendedTiger::ExtendedTiger(options_t &options) {
	//Initial state of 0 is sitting, 1 is standing
	state = 0;

	//Setting up initial reward and initial observation
	m_observation = 0;
	m_reward = 0;

	// Set up which door is which
	gold_door = (rand01() < 0.5) ? 1 : 2;
	tiger_door = ((gold_door + 1) % 2) + 1;
}


void ExtendedTiger::performAction(action_t action) {
	// Rewards are normalised to 100 to be positive
	// Actions
	// 1 is stand
	// 2 is listen
	// 3 is open door 1
	// 4 is open door 2
	switch(action){
		// Cases for actions
		case 0:
			// When action is standing, if already standing, take less reward for invalid action
			if (state == 1) {
				m_reward = 90;
				m_observation = (pow(2,2)*state);
			} else{
				state = 1;
				m_reward = 99;
				m_observation = (pow(2,2)*state);
			}
			break;
		case 1:
			// When action is listen, take less reward if invalid action (while standing), else gain observation
			if (state == 1) {
				m_reward = 90;
				m_observation = (pow(2,2)*state);
			} else{
				m_observation = ((rand01() < 0.85) ? tiger_door : gold_door ) + (pow(2,2)*state);
				m_reward = 99;
				state = 0;
			}
			break;
		case 2:
			// When action is open door, less reward if sitting, then check if door is gold or tiger
			if (state == 0) {
				m_reward = 90;
				m_observation = (pow(2,2)*state);
			} else {
				m_reward = (1 == gold_door) ? 130 : 0;
				m_observation = (pow(2,2)*state);
				state = 0;
			}
			break;
		case 3:
			// When action is open door, less reward if sitting, then check if door is gold or tiger
			if (state == 0) {
				m_reward = 90;
				m_observation = (pow(2,2)*state);
			} else {
				m_reward = (2 == gold_door) ? 130 : 0;
				m_observation = (pow(2,2)*state);
				state = 0;
			}
			break;
	}

}

void KuhnPoker::reset_game() {
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
	m_observation = (opponent_action ? pow(2,3): 0) + ((agent_card < 2) ? ((agent_card < 1) ? pow(2,0): pow(2,1)) : pow(2,2));
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
	m_observation = (opponent_action ? pow(2,3): 0) + ((agent_card < 2) ? ((agent_card < 1) ? pow(2,0): pow(2,1)) : pow(2,2));
}


void KuhnPoker::performAction(action_t action) {

	// Using normalized reward to 4

	// Agent takes action, 1 to bet or 0 to pass

	// amount of chips put in, and amoutn of chips in play both increase if agent bets
	agent_chips_put_in += action;
	chips_in_play += action;


	// if actions are equal showdown occurs, 
	// else if opponents bets he wins, 
	// else opponent has choice to bet and showdown occurs or pass and let agent win

	if (opponent_action == action) {
		//showdown
		if (agent_card > opponent_card) {
			//agent wins
			m_reward = 2 + chips_in_play;
			// Reset the game
			reset_game();
		} else {
			//opponent wins
			m_reward = 2 - agent_chips_put_in; 
			// Reset the game
			reset_game();
		}

	} else if (opponent_action == 1) {
		//opponent wins
		m_reward = 2 - agent_chips_put_in; 
		// Reset the game
		reset_game();
	} else {
		if (rand01() < alpha + 1/3.0) {
			// opponent calls then showdown
			chips_in_play += 1;
			if (agent_card > opponent_card) {
				//agent wins
				m_reward = 2 + chips_in_play;
				// Reset the game
				reset_game();
			} else {
				//opponent wins
				m_reward = 2 - agent_chips_put_in; 
				// Reset the game
				reset_game();
			}

		} else {
			//opponent folds and agent wins
			m_reward = 2 + chips_in_play;
			// Reset the game
			reset_game();
		}
	}
	// m_observation = 0;
	
}

