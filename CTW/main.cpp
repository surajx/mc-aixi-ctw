/******************************************************************************
** Unit Tests for CTW
**
** Author: Suraj Narayanan Sasikumar
*******************************************************************************/

// #include <random>
#include <iostream>
#include <math.h>

#include "ContextTree.hpp"

#include "../common/constants.hpp"

using namespace std;

double rand01() {
	return (double)rand() / (double)RAND_MAX;
}

int main(int argv, char* argc[]) {
  uint_t depth = 3;
  ContextTree* cTree = new ContextTree(depth);
  CTNode* rootNode = cTree->getRootNode();

  // mt19937 randomizer(61874);

  symbol_t randSym;
  uint_t totSyms = 5000;
  uint_t one_cnt = 0;
  uint_t zero_cnt = 0;
  for (int i = 0; i < totSyms; i++) {
    // randSym = (randomizer)() % 2;
    randSym = rand01() < 0.8 ? 1 : 0;
    cTree->update(randSym);
    if(randSym==SYMBOL_1) ++one_cnt;
    else if(randSym==SYMBOL_0) ++zero_cnt;
  }
  cout << "num ones: " << one_cnt <<endl;
  cout << "num zero: " << zero_cnt <<endl;
  cout << "next symbol: " << cTree->predictSymbol() <<endl;

// // Past Observation
// cTree->update(SYMBOL_0);
// cTree->update(SYMBOL_1);
// cTree->update(SYMBOL_0);

// // New observations
// cTree->update(SYMBOL_0);
// // cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
// // cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// cTree->update(SYMBOL_1);
// // cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
// // cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// cTree->update(SYMBOL_1);
// cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
// cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// cout << "1-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
// cout << "1-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) <<
// endl;
// cTree->update(SYMBOL_0);
// cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
// cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// // cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) <<
// endl;
// // cout << "0-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted())
// << endl;
// cTree->update(SYMBOL_1);
// cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
// cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// // cout << "1-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) <<
// endl;
// // cout << "1-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted())
// << endl;
// cTree->update(SYMBOL_0);
// cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
// cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
// // cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
// endl;
// // cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
// << endl;
// // cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) <<
// endl;
// // cout t"},{"label":"tuple_element","kind":7,"detail":"tuple_element<1,
// std::pair<_Tp1,_Tp2>>","documentation":"File:
// /usr/include/c++/5/utility","insertText":"tuple_element"},{"label":"tuple_element","kind":7,"detail":"tuple_element<0,
// std::pair<_Tp1,_Tp2>>","documentation":"File:
// /usr/include/c++/5/utility","insertText":"tuple_element"},{"label":"to_string","kind":3,"detail":"string
// to_string(double)","documentation":"File:
// /usr/include/c++/5/bits/basic_string.h","insertText":"to_string"},{"label":"type","kind":7,"detail":"decltype(__test<_Tp,
// _Up>(0)) type","documentation":"File: /usr/include/c++/5/type_
  // << endl;
  // cTree->update(SYMBOL_0);
  // cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
  // cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
  // // cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) <<
  // endl;
  // // cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted())
  // << endl;
  // // cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) <<
  // endl;
  // // cout << "0-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted())
  // << endl;
}