/****************************************************************************************
** TODO: Documentation
**
** Author: Suraj Narayanan Sasikumar
***************************************************************************************/

#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <map>
#include <vector>

// symbols that can be predicted
// TODO: think about using boost dynamic bitset library. Stick with this till
// things take off.
typedef bool symbol_t;

// a list of symbols
typedef std::vector<symbol_t> symbol_list_t;

// describe the reward accumulated by an agent
typedef double reward_t;

// describe a percept (observation or reward)
typedef unsigned int percept_t;

// describe the lifetime of an agent
typedef unsigned long long lifetime_t;

// describes an agent action
typedef unsigned int action_t;

// All information about variable content should be inferred from variable name
// All information about variable type should be inferred from type name,
// I strongly feel that both these should not be muddled.
typedef unsigned int uint_t;

// the program's keyword/value option pairs
typedef std::map<std::string, std::string> options_t;

#endif