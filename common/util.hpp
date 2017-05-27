/****************************************************************************************
** A collection of functions to be used throughout the MC-AIXI-CTW program.
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/


#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../common/types.hpp"

// Return a number uniformly between [0, 1]
double rand01();

// Return a random integer between [0, end)
unsigned int randRange(unsigned int end);

// Return a random number between [start, end)
int randRange(int start, int end);

// Extract a value from a string
template <typename T>
void strExtract(std::string& str, T& val) {
  std::istringstream iss(str);
  iss >> val;
}

template <typename T>
T strExtract(std::string& str) {
  T val;
  strExtract(str, val);
  return val;
}

// encode/decode values to/from symbol lists
unsigned int decode(const symbol_list_t& symlist, unsigned int bits);
void encode(symbol_list_t& symlist, unsigned int value, unsigned int bits);

// number of distinct percepts (size of the percept space)
unsigned int countPercepts(unsigned int num_obs, unsigned num_rew);

// encode an observation and a reward as a single observation-reward
percept_t perceptIndex(percept_t obs,
                       percept_t rew,
                       uint_t numObservationBits,
                       uint_t numRewardBits);

#endif  // __UTIL_HPP__
