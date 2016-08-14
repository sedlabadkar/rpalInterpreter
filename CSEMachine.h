/*
 * CSEMachine.h
 *
 *  Created on: Apr 21, 2016
 *      Author: sachin
 */

#ifndef RPAL_COMPILER_CSEMACHINE_H_
#define RPAL_COMPILER_CSEMACHINE_H_
#include <vector>
#include <stack>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "Environment.h"

using namespace std;

static vector <string> inbuiltFuncVector = {
                                            "Order",
                                            "Print",
                                            "Isinteger",
                                            "Istruthvalue",
                                            "Isstring",
                                            "Istuple",
                                            "Isfunction"
                                            "Isdummy",
                                            "Stem",
                                            "Stern",
                                            "Conc",
                                            "Conc2",
                                            "ItoS",
                                            "Null"
                                            };

class CSEMachine {
    void flattenDeltaThen(treeNode*, Control *,vector<Control *> *);
    void flattenDeltaElse(treeNode*, Control *,vector<Control *> *);
    void flattenLAMBDA(treeNode*, Control *,vector<Control *> *);
    void flattenTernary(treeNode*, Control *,vector<Control *> *);
    void flattenTree(treeNode*, Control *,vector<Control *> *);
    void init(treeNode*);
    void deltaPrint();
    void applyBinaryOPR(int);
    void applyThisRator(Control*);
    void printCS();
    bool checkInbuilt(string);
    void escapePrintStr(string);
    void rule411(Control*, Control*, Control*, Environment*, int);
    void rule12(Control*, Control*, Control*, Environment*, int);
    void rule13(Control*, Control*, Control*, Environment*, int);
    void rule10(Control*, Control*, Control*, Environment*, int);
    void handleNeg(Control*, Control*, Control*, Environment*, int);
    void handleEnv(Control*, Control*, Control*, Environment*, int);
    void handleTau(Control*, Control*, Control*, Environment*, int);
    void handleBeta(Control*, Control*, Control*, Environment*, int);
    void handleName(Control*, Control*, Control*, Environment*, int);
    void handleGAMMA(Control*, Control*, Control*, Environment*, int);

    treeNode* top;
    vector<Control *> *deltas;
    Control *rootDelta;
    int numEnvironment;
    vector<Control *> control;
    stack<Control *> execStack;
    Environment *currEnvironment;
    Environment *tempEnvironment;
    Environment *PE;
    stack<Environment*> environmentStack;
    map<int, Environment *> environmentMap;
public:
    CSEMachine();
    CSEMachine(treeNode*);
    virtual ~CSEMachine();
    Environment* primitiveEnv;
    void run(treeNode*);
    Environment* createNewEnvironment();
    void execCSE();
};


#endif /* RPAL_COMPILER_CSEMACHINE_H_ */
