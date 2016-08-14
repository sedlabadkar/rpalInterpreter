/*
 * treeNode.h
 *
 *  Created on: Apr 20, 2016
 *      Author: sachin
 */

#ifndef RPAL_COMPILER_TREENODE_H_
#define RPAL_COMPILER_TREENODE_H_
#include "lexer.h"
#include <stack>


class treeNode{

public:
    enum Type {
        LAMBDA = 1 ,
        WHERE = 2 ,
        TAU = 3 ,
        AUG = 4 ,
        TERNARY = 5 ,
        OR = 6 ,
        AND_LOGICAL = 7 ,
        NOT = 8 ,
        GR = 9 ,
        GE = 10 ,
        LS = 11 ,
        LE = 12 ,
        EQ = 13 ,
        NE = 14 ,
        ADD = 15 ,
        SUBTRACT = 16 ,
        NEG = 17 ,
        MULTIPLY = 18 ,
        DIVIDE = 19 ,
        EXPONENTIAL = 20 ,
        AT = 21 ,
        GAMMA = 22 ,
        TRUE = 23 ,
        FALSE = 24 ,
        NIL = 25 ,
        DUMMY = 26 ,
        WITHIN = 27 ,
        AND = 28 ,
        REC= 29 ,
        BINDING = 30 ,
        FCN_FORM = 31 ,
        PARANTHESES = 32 ,
        COMMA = 33 ,
        LET = 34 ,
        IDENTIFIER = 35 ,
        STRING = 36 ,
        INTEGER = 37,
        YSTAR = 38
        };

    string nodeString;
    treeNode* childNode = NULL;
    treeNode* siblingNode = NULL;
    int type;

};


#endif /* RPAL_COMPILER_TREENODE_H_ */
