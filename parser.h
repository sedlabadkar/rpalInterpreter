/*
 * parser.h
 *
 *  Created on: Mar 1, 2016
 *      Author: sachin
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "lexer.h"
#include <stack>
#include "TreeStandardizer.h"
#include "CSEMachine.h"

using namespace std;

class parser {
public:

	parser (lexer*);
	void printAST();
	void printST();
    void evaluateProg();
	virtual ~parser();

private:
    lexer* lex;
    stack <treeNode*> treeStack;
    token *nextToken;

	void E();
	void Ew();
	void T();
	void Ta();
	void Tc();
	void B();
	void Bt();
	void Bs();
	void Bp();
	void A();
	void At();
	void Af();
	void Ap();
	void R();
	void Rn();
	void D();
	void Da();
	void Dr();
	void Db();
	void Vb();
	void Vl();

	void parse();
	void standardize(treeNode*);
    void buildTree(string, int); //Tree node name & child nodes num
    void buildTree(string, int, int);
    void treePrettyPrint(treeNode*, int);
    void read(string);
	bool isKeyword(string);
	string to_s(treeNode*);
};

#endif /* PARSER_H_ */
