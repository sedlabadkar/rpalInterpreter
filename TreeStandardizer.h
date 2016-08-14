/*
 * TreeStandardizer.h
 *
 *  Created on: Apr 20, 2016
 *      Author: sachin
 */

#ifndef TREESTANDARDIZER_H_
#define TREESTANDARDIZER_H_

#include "lexer.h"
#include "treeNode.h"

using namespace std;

class TreeStandardizer {
    void standardize(treeNode*);
    void standardizeLET(treeNode*);
    void standardizeWHERE(treeNode*);
    void standardizeWITHIN(treeNode*);
    void standardizeREC(treeNode*);
    void standardizeFCNFORM(treeNode*);
    void standardizeLAMBDA(treeNode*);
    void standardizeAND(treeNode*);
    void standardizeAT(treeNode*);

public:
    TreeStandardizer(treeNode* topNode);
    virtual ~TreeStandardizer();
};

#endif /* TREESTANDARDIZER_H_ */
