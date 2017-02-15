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

  symbol_t randSym;
  uint_t totSyms = 5000;
  uint_t one_cnt = 0;
  uint_t zero_cnt = 0;
  double p = 0.5;
  for (int i = 0; i < totSyms; i++) {
    randSym = rand01() < p ? 1 : 0;
    cTree->update(randSym);
    if (randSym == SYMBOL_1)
      ++one_cnt;
    else if (randSym == SYMBOL_0)
      ++zero_cnt;
  }
  cout << "num ones: " << one_cnt << endl;
  cout << "num zero: " << zero_cnt << endl;
  cout << "P(x_t=1|history): " << cTree->predict(SYMBOL_1) << endl;
  
  cTree->clear();
  cout << "Cleared ContextTree." << endl;
  one_cnt = 0;
  zero_cnt = 0;

  for (int i = 0; i < totSyms; i++) {
    randSym = rand01() < p ? 1 : 0;
    cTree->update(randSym);
    if (randSym == SYMBOL_1)
      ++one_cnt;
    else if (randSym == SYMBOL_0)
      ++zero_cnt;
  }
  cout << "num ones: " << one_cnt << endl;
  cout << "num zero: " << zero_cnt << endl;
  cout << "P(x_t=1|history): " << cTree->predict(SYMBOL_1) << endl;

  symbol_list_t symbol_list;
  // for (int i = 0; i < 5; i++) {
  //   randSym = rand01() < p ? 1 : 0;
  //   cout << randSym;
  //   symbol_list.push_back(randSym);
  // }

  // cout << "::P(x_t=seq|history): " << cTree->predict(symbol_list) << endl;

  // cout << "Predicted Symbol: " << cTree->predictSymbol() << endl;

  // symbol_list.clear();

  cTree->genNextSymbols(symbol_list, 1000);
  for (int i = 0; i < symbol_list.size(); i++) {
    cout << symbol_list[i];
  }
  cout << endl;
}