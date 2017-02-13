#include <iostream>
#include <math.h>

#include "ContextTree.hpp"

#include "../common/constants.hpp"

using namespace std;

int main(int argv, char* argc[]) {
  uint_t depth = 3;
  ContextTree* cTree = new ContextTree(depth);
  CTNode* rootNode = cTree->getRootNode();

  // Past Observation  
  cTree->update(SYMBOL_0);
  cTree->update(SYMBOL_1);
  cTree->update(SYMBOL_0);
  
  // New observations
  cTree->update(SYMBOL_0);
  cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;    
  cTree->update(SYMBOL_1);
  cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;  
  cTree->update(SYMBOL_1);
  cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;
  cout << "1-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
  cout << "1-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) << endl;
  cTree->update(SYMBOL_0);
  cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;
  cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
  cout << "0-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) << endl;
  cTree->update(SYMBOL_1);
  cout << "1KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "1W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "1-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "1-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;
  cout << "1-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
  cout << "1-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) << endl;
  cTree->update(SYMBOL_0);
  cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;
  cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
  cout << "0-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) << endl;
  cTree->update(SYMBOL_0);
  cout << "0KT:" << exp(rootNode->getLogProbKT()) << endl;
  cout << "0W:" << exp(rootNode->getLogProbWeighted()) << endl;
  cout << "0-0KT:" << exp(rootNode->child(SYMBOL_0)->getLogProbKT()) << endl;
  cout << "0-0W:" << exp(rootNode->child(SYMBOL_0)->getLogProbWeighted()) << endl;
  cout << "0-1KT:" << exp(rootNode->child(SYMBOL_1)->getLogProbKT()) << endl;
  cout << "0-1W:" << exp(rootNode->child(SYMBOL_1)->getLogProbWeighted()) << endl;
}