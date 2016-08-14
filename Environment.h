/*
 * Environment.h
 *
 *  Created on: Apr 22, 2016
 *      Author: sachin
 */

#ifndef RPAL_COMPILER_ENVIRONMENT_H_
#define RPAL_COMPILER_ENVIRONMENT_H_

#include <map>
#include <string>
#include "Control.h"

using namespace std;

class Environment {
public:
    int id;
    Environment *parent;//this ist primitive
    void assignParent(Environment* );
    Control* lookup(string);
    void prettyPrint();
    Environment(int);
    virtual ~Environment();
    map<string, Control *> symbolTable;
};

#endif /* RPAL_COMPILER_ENVIRONMENT_H_ */
