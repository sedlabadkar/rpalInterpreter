/*
 * token.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: sachin
 */

#include "token.h"

token::token() {
	tokType = 0;
	tokValue = "";
	lineCount = 0;
	charCount = 0;

}

token::~token() {
}
