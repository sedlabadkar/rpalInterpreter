/*
 * TreeStandardizer.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: sachin
 */

#include "TreeStandardizer.h"

TreeStandardizer::TreeStandardizer(treeNode* topNode) {
    // TODO Auto-generated constructor stub
    standardize(topNode);
}

TreeStandardizer::~TreeStandardizer() {
    // TODO Auto-generated destructor stub
}

void TreeStandardizer::standardizeLET(treeNode* topNode){
    treeNode* p;
    if (STANDARDIZERLOGS) printf ("CASE LET\n");

    if(treeNode::BINDING != topNode->childNode->type){
        printf ("Subtree not standardised: case LET");
    }

    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";
    topNode->childNode->type = treeNode::LAMBDA;
    topNode->childNode->nodeString = "lambda";
    p = topNode->childNode->siblingNode;
    topNode->childNode->siblingNode = topNode->childNode->childNode->siblingNode;
    topNode->childNode->childNode->siblingNode = p;

}

void TreeStandardizer::standardizeWHERE(treeNode* topNode){
    treeNode* p, *x, *e;
    if (STANDARDIZERLOGS) printf ("CASE WHERE\n");
    if(treeNode::BINDING != topNode->childNode->siblingNode->type){
      printf("Subtree not standardised: case WHERE");
    };
    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";
    p = topNode->childNode;
    x = topNode->childNode->siblingNode->childNode;
    e = topNode->childNode->siblingNode->childNode->siblingNode;
    p->siblingNode = NULL;
    x->siblingNode = NULL;
    topNode->childNode = new treeNode();
    topNode->childNode->type = treeNode::LAMBDA;
    topNode->childNode->nodeString = "lambda";
    topNode->childNode->siblingNode = e;
    topNode->childNode->childNode = x;
    x->siblingNode = p;
}

void TreeStandardizer::standardizeWITHIN(treeNode* topNode){
    treeNode *x1, *x2, *e1;
    if (STANDARDIZERLOGS) printf ("CASE WITHIN\n");
    if(treeNode::BINDING != topNode->childNode->type || treeNode::BINDING != topNode->childNode->siblingNode->type){
        printf("Subtree not standardised: case WITHIN");
    };
    topNode->type = treeNode::BINDING;
    x1 = topNode->childNode->childNode;
    x2 = topNode->childNode->siblingNode->childNode;
    e1 = x1->siblingNode;
    x1->siblingNode = x2->siblingNode;
    x2->siblingNode = new treeNode();
    x2->siblingNode->type = treeNode::GAMMA;
    x2->siblingNode->nodeString = "gamma";
    x2->siblingNode->childNode = new treeNode();
    x2->siblingNode->childNode->type = treeNode::LAMBDA;
    x2->siblingNode->childNode->nodeString = "lambda";
    x2->siblingNode->childNode->siblingNode = e1; // the new lambda's right
    x2->siblingNode->childNode->childNode = x1; // the new lambda's left has now x1 who's first sibling is e2
    topNode->childNode = x2;

}

void TreeStandardizer::standardizeREC(treeNode* topNode){
    treeNode *x, *e, *temp;
    if (STANDARDIZERLOGS) printf ("CASE REC\n");
    if(treeNode::BINDING != topNode->childNode->type){
        printf("Subtree not standardised: case REC");
    };
    topNode->type = treeNode::BINDING;
    x = topNode->childNode->childNode;
    e = topNode->childNode->childNode->siblingNode;
    x->siblingNode = NULL;
    topNode->childNode = x; //removing the reference to the old = node
    temp = new treeNode();
    temp->type = treeNode::GAMMA;
    temp->nodeString = "gamma";
    x->siblingNode = temp;
    temp->childNode = new treeNode();
    temp->childNode->type = treeNode::YSTAR;
    temp->childNode->nodeString = "<Y*>";
    temp->childNode->siblingNode = new treeNode();
    temp->childNode->siblingNode->type = treeNode::LAMBDA;
    temp->childNode->siblingNode->nodeString = "lambda";
    temp->childNode->siblingNode->childNode = new treeNode();
    temp->childNode->siblingNode->childNode->type = x->type;
    temp->childNode->siblingNode->childNode->nodeString = x->nodeString;
    temp->childNode->siblingNode->childNode->siblingNode = e;
}

void TreeStandardizer::standardizeFCNFORM(treeNode* topNode){
    treeNode *temp, *newNode;
    if (STANDARDIZERLOGS) printf ("CASE FCN_FORM\n");
    topNode->type = treeNode::BINDING;
    topNode->nodeString = "=";
    temp = topNode->childNode;
    while (temp->siblingNode->siblingNode != NULL){
        newNode = new treeNode();
        if (STANDARDIZERLOGS) printf ("newNOde allocated at %p\n", newNode);
        newNode->type = treeNode::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = newNode->childNode;
    }
}

void TreeStandardizer::standardizeLAMBDA(treeNode* topNode){
    treeNode *temp, *newNode;
    if (STANDARDIZERLOGS) printf ("CASE LAMBDA\n");
    temp = topNode->childNode;
    while (temp->siblingNode->siblingNode != NULL){
        newNode = new treeNode();
        newNode->type = treeNode::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = temp->siblingNode;
    }
}

void TreeStandardizer::standardizeAND(treeNode* topNode){
    treeNode* temp;
    treeNode* tauNode, *commaNode, **currentTau, **currentComma;
    if (STANDARDIZERLOGS) printf ("CASE AND\n");
    topNode->type = treeNode::BINDING;
    topNode->nodeString = "=";
    temp = topNode->childNode;
    tauNode = new treeNode();
    commaNode = new treeNode();
    tauNode->type = treeNode::TAU;
    tauNode->nodeString = "tau";
    tauNode->childNode = NULL;
    tauNode->siblingNode = NULL;
    commaNode->type = treeNode::COMMA;
    commaNode->nodeString = ",";
    commaNode->childNode = NULL;
    commaNode->siblingNode = NULL;
    topNode->childNode = commaNode;
    topNode->childNode->siblingNode = tauNode;
    currentTau = &(tauNode->childNode);
    currentComma = &(commaNode->childNode);
    while (temp != NULL){
        *currentTau = temp->childNode->siblingNode;
        temp->childNode->siblingNode = NULL;
        *currentComma = temp->childNode;
        temp = temp->siblingNode;
        currentComma = &((*currentComma)->siblingNode); // Can your code BE any more UGLY?
        currentTau = &((*currentTau)->siblingNode);
    }

}

void TreeStandardizer::standardizeAT(treeNode* topNode){
    treeNode* E1, *N, *E2;
    if (STANDARDIZERLOGS) printf ("CASE AT\n");
    E1 = topNode->childNode;
    N = E1->siblingNode;
    E2 = N->siblingNode;
    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";
    topNode->childNode = new treeNode();
    topNode->childNode->type = treeNode::GAMMA;
    topNode->childNode->nodeString = "gamma";
    topNode->childNode->siblingNode = E2;
    topNode->childNode->childNode = N;
    N->siblingNode = NULL;
    topNode->childNode->childNode->siblingNode = E1;
    E1->siblingNode = NULL;

}

//It doesn't get uglier than this
void TreeStandardizer::standardize(treeNode* topNode){
    treeNode *p, *e, *e1, *e2, *x, *x1, *x2, *n, *temp, *new_temp;
    string origString = topNode->nodeString;
    if (STANDARDIZERLOGS) printf ("Standardizing Node = %s\n", topNode->nodeString.c_str());
    if (topNode->childNode != NULL)
        standardize(topNode->childNode);
    if (topNode->siblingNode != NULL)
        standardize (topNode->siblingNode);

    switch(topNode->type){
    case treeNode::LET:
        standardizeLET(topNode);
        break;
    case treeNode::WHERE:
        standardizeWHERE(topNode);
        break;
    case treeNode::WITHIN:{
        standardizeWITHIN(topNode);
        break;
    }
    case treeNode::REC:{
        standardizeREC(topNode);
        break;
    }
    case treeNode::FCN_FORM:{
        standardizeFCNFORM(topNode);
        break;
    }
    case treeNode::LAMBDA:
        standardizeLAMBDA(topNode);
        break;
    case treeNode::AND:
        standardizeAND(topNode);
        break;
    case treeNode::AT:
        standardizeAT(topNode);
        break;
    default:
        if (0) printf ("Nothing to do. TopNode is %s\n", origString.c_str());
    }
    if (0) printf ("Done with node %s\n", origString.c_str());
}
