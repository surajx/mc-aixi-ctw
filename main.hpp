#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <fstream>
#include <map>
#include <string>
#include <vector>

// Streams for logging (defined in main.cpp)
extern std::ofstream logger;              // A verbose human-readable log
extern std::ofstream csvLogger;           // A compact comma-separated value log
extern std::ofstream csvFinalEvalLogger;  // SeparateLogger for evaluation
extern std::ofstream csvMidEvalLogger;    // SeparateLogger for evaluation

#endif // __MAIN_HPP__
