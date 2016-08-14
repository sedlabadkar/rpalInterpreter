/*
 * lexer.h
 *
 *  Created on: Mar 1, 2016
 *      Author: sachin
 */

#ifndef LEXER_H_
#define LEXER_H_
using namespace std;
#define LEXLOGS 0
#define PARSERLOGS 0
#define STANDARDIZERLOGS 0
#define MAIN_DEBUG_LOGS 0

#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "token.h"

class lexer {
public:
	lexer(std::ifstream*);
	virtual ~lexer();
	token* getNextToken();
	void lexerReset();

private:
    ifstream* sourceFile;
    int lineCount, charCount;
    string tokenIdentifier();
    string tokenInteger();
    string tokenStrings();
    string tokenSpaces();
    string tokenComment();
    string tokenOperator();
    string tokenPunction();

	bool isPunction(char);
	bool isOperatorSymbol(char);
	bool isSpaces(char);
	bool isEOL(char);
	bool isEOF(char);
	bool isCommentChar(char);
	bool isKeyword(string);
};

#endif /* LEXER_H_ */
