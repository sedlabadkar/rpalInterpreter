/*
 * parser.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: sachin
 */

#include "parser.h"

parser::parser(lexer* lexr){
	this->lex = lexr;
	nextToken = NULL;
}

parser::~parser() {
}


void parser::printAST(){
	parse();
	if (!treeStack.empty())
		treePrettyPrint(treeStack.top(), 0);
	lex->lexerReset();
}

void parser::printST(){
    parse();
    TreeStandardizer *ts = new TreeStandardizer(treeStack.top());
    if (!treeStack.empty())
        treePrettyPrint(treeStack.top(), 0);
    //evaluateProg();
    lex->lexerReset();
}


void parser::evaluateProg(){
    //Evaluation will only happen if the interpreter was called without switches
    parse();
    TreeStandardizer *ts = new TreeStandardizer(treeStack.top());
    //No need to print
    CSEMachine* cse = new CSEMachine();
    cse->run(treeStack.top());
}

void parser::parse(){
	if (PARSERLOGS) printf ("Initiating parsing\n");
	//spaces and comments are not considered
	do {
		nextToken = lex->getNextToken();
	}while (nextToken->tokType == TOK_DELETE);
	E();
	if (!treeStack.empty() && treeStack.size() != 1){
		printf ("Error: Stack not empty at the end of parsing\n");
		exit(0);
	} else if (lex->getNextToken()->tokType != TOK_EOF){
		printf ("Error: Parsing finished input still remaining\n");
		exit(0);
	}
	if (PARSERLOGS) printf ("Parsing Finished. AST Generated\n");
}

bool parser::isKeyword(string val){
    if (val == "in" || val == "where" || val == "." || val == "aug" || val == "and" || val == "or"
		|| val == "&" || val == "not" || val == "gr" || val == "ge" || val == "ls" || val == "le"
		|| val == "eq" || val == "ne" || val == "+" || val == "-" || val == "*" || val == "/"
		|| val == "**" || val == "@" || val == "within" || val == "rec" || val == "let" || val == "fn")
    return true;
    else
      return false;
}

void parser::read(string tokStr){
    if (PARSERLOGS) printf ("read() args %s\\, match %s\\\n", tokStr.c_str(), nextToken->tokValue.c_str());
    if (!(nextToken->tokValue == tokStr)){
        printf ("At L:C::%d:%d Expected '%s', received '%s'\n", nextToken->lineCount, nextToken->charCount,
        tokStr.c_str(), nextToken->tokValue.c_str());
        exit(0);
    }
    //Spaces and comments are not considered
    do {
        nextToken = lex->getNextToken();
    }while (nextToken->tokType == TOK_DELETE);
}

void parser::buildTree(string nodeStr, int numChildNodes, int type){
    if (PARSERLOGS) printf ("buildTree. Current Tree Size = %d, Node to add = %s, numChild = %d\n", (int)treeStack.size(), nodeStr.c_str(), numChildNodes);
    int finalSize = treeStack.size() - numChildNodes + 1;
    treeNode* newNode = new treeNode();
    treeNode* tempNode = new treeNode();
    newNode->nodeString = nodeStr;
    newNode->type = type;
    if (numChildNodes == 0){
        treeStack.push(newNode);
        if (PARSERLOGS) printf ("New Node added. Stack Size = %d\n", (int)treeStack.size());
        return;
    }
    if (treeStack.empty()){
        if (PARSERLOGS) printf ("We were asked to pop but the stack is empty\n");
        return;
    } else {
        while ((numChildNodes - 1) > 0){
            if (!treeStack.empty()){
                tempNode = treeStack.top();
                treeStack.pop();
                if (treeStack.size() != 0){
                    treeStack.top()->siblingNode = tempNode;
                } else if (treeStack.size() == 0) { //Stack cannot be empty here. We have one more element to pop before we can build/push the requested tree
                    printf ("Parse Error: Empty Stack\n");
                    exit(0);
                }
                numChildNodes--;
            } else {
                if (PARSERLOGS) printf ("Stack size is less than numChildNodes. Abort.\n");
                return;
            }
        }
        tempNode = treeStack.top();
        newNode->childNode = tempNode;
        treeStack.pop();
    }
    treeStack.push(newNode);
    if (finalSize != treeStack.size()){
        if (PARSERLOGS) printf ("Something went horribly wrong in build tree function\n");
    }
    if (PARSERLOGS) printf ("BuildTree Success. Stack Size = %d\n", (int)treeStack.size());
}

string parser::to_s(treeNode* node)
 {
   string str ;
   switch(node->type)
   {
   case treeNode::IDENTIFIER:
       return "<ID:" + node->nodeString + ">";
   case treeNode::INTEGER:
       return "<INT:" + node->nodeString + ">";
   case treeNode::STRING:
       return "<STR:" + node->nodeString + ">";
   default:
       return node->nodeString;
   }
   return NULL;
 }

void parser::treePrettyPrint(treeNode* topNode, int numDots){
	int numDots1 = numDots;
	while (numDots1 > 0) {
		printf (".");
		numDots1--;
	}
	printf ("%s\n", to_s(topNode).c_str());

	if (topNode->childNode != NULL){
		treePrettyPrint(topNode->childNode, numDots + 1);
	}
	if (topNode->siblingNode != NULL){
		treePrettyPrint(topNode->siblingNode, numDots);
	}
}

/* E    -> ’let’ D ’in’ E                          => ’let’
     -> ’fn’ Vb+ ’.’ E                          => ’lambda’
     ->  Ew;
*/
void parser::E(){
	if (PARSERLOGS) printf ("Proc E next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "let")
	{
		read("let");
		D();
		read ("in");
		E();
		buildTree("let", 2, treeNode::LET);
	} else if (nextToken->tokValue == "fn"){
		read("fn");
		int n = 0;
		do {
			Vb();
			n++;
		}while(nextToken->tokValue == "(" || nextToken->tokType == TOK_IDENTIFIER);
		read(".");
		E();
		buildTree("lambda", n+1, treeNode::LAMBDA);
	} else {
		Ew();
	}
}

/* Ew   -> T ’where’ Dr                            => ’where’
   -> T;*/
 void parser::Ew(){
	if (PARSERLOGS) printf ("Proc Ew next token = %s\n", nextToken->tokValue.c_str());
 	T();
 	if (nextToken->tokValue == "where")
 	{
 		read("where");
 	 	Dr();
 	 	buildTree("where",2, treeNode::WHERE);
 	}
 }

 /* T    -> Ta ( ’,’ Ta )+                          => ’tau’
     -> Ta ;
  */
 void parser::T(){
	 if (PARSERLOGS) printf ("Proc T next token = %s\n", nextToken->tokValue.c_str());
	 int n = 0;
	 Ta();
	 if (nextToken->tokValue == ","){
		 do {
			 read(",");
			 Ta();
			 n++;
		 }while (nextToken->tokValue == ",");
		 buildTree("tau", n+1, treeNode::TAU);
	 }
 }

 /* Ta   -> Ta ’aug’ Tc                             => ’aug’
     -> Tc ;
  */
 void parser::Ta(){
	 if (PARSERLOGS) printf ("Proc Ta next token = %s\n", nextToken->tokValue.c_str());
	 Tc();
	 while (nextToken->tokValue == "aug"){
		 read("aug");
		 Tc();
		 buildTree("aug", 2, treeNode::AUG);
	 }
 }

 /* Tc   -> B ’->’ Tc ’|’ Tc                      => '->'
     -> B ;
  */
 void parser::Tc(){
	 if (PARSERLOGS) printf ("Proc Tc next token = %s\n", nextToken->tokValue.c_str());
	 B();
	 if (nextToken->tokValue == "->"){
		 read("->");
		 Tc();
		 read("|");
		 Tc();
		 buildTree("->", 3, treeNode::TERNARY);
	 }
 }

/* B    -> B ’or’ Bt                               => ’or’
     -> Bt ;
  */
void parser::B(){
	if (PARSERLOGS) printf ("Proc B next token = %s\n", nextToken->tokValue.c_str());
	Bt();
	while (nextToken->tokValue == "or"){
		read ("or");
		Bt();
		buildTree("or", 2, treeNode::OR);
	}
}

/*  Bt   -> Bt ’&’ Bs                               => ’&’
     -> Bs ;
  */
void parser::Bt(){
	if (PARSERLOGS) printf ("Proc Bt next token = %s\n", nextToken->tokValue.c_str());
	Bs();
	while (nextToken->tokValue == "&"){
		read("&");
		Bs();
		buildTree("&", 2, treeNode::AND_LOGICAL);
	}
}

/* Bs   -> ’not’ Bp                                => ’not’
     -> Bp ;
  */
void parser::Bs(){
	if (PARSERLOGS) printf ("Proc Bs next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "not"){
		read ("not");
		Bp();
		buildTree("not", 1, treeNode::NOT);
	}else {
		Bp();
	}
}

/* Bp   -> A (’gr’ | ’>’ ) A                       => ’gr’
     -> A (’ge’ | ’>=’) A                       => ’ge’
     -> A (’ls’ | ’<’ ) A                       => ’ls’
     -> A (’le’ | ’<=’) A                       => ’le’
     -> A ’eq’ A                                => ’eq’
     -> A ’ne’ A                                => ’ne’
     -> A ;
  */
void parser::Bp(){
	if (PARSERLOGS) printf ("Proc Bp next token = %s\n", nextToken->tokValue.c_str());
	A();
	if (nextToken->tokValue == "gr" || nextToken->tokValue == ">"){
		if (nextToken->tokValue == "gr")
			read("gr");
		else if (nextToken->tokValue == ">")
			read(">");
		A();
		buildTree("gr", 2, treeNode::GR);
	}else if (nextToken->tokValue == "ge" || nextToken->tokValue == ">="){
		if (nextToken->tokValue == "ge")
			read("ge");
		else if (nextToken->tokValue == ">=")
			read(">=");
		A();
		buildTree("ge", 2, treeNode::GE);
	}else if (nextToken->tokValue == "ls" || nextToken->tokValue == "<"){
		if (nextToken->tokValue == "ls")
			read("ls");
		else if (nextToken->tokValue == "<")
			read("<");
		A();
		buildTree("ls", 2, treeNode::LS);
	}else if (nextToken->tokValue == "le" || nextToken->tokValue == "<="){
		if (nextToken->tokValue == "le")
			read("le");
		else if (nextToken->tokValue == "<=")
			read("<=");
		A();
		buildTree("le", 2, treeNode::LE);
	}else if (nextToken->tokValue == "eq"){
		read("eq");
		A();
		buildTree("eq", 2, treeNode::EQ);
	}else if (nextToken->tokValue == "ne"){
		read("ne");
		A();
		buildTree("ne", 2, treeNode::NE);
	}
}

/* A    -> A ’+’ At                                => ’+’
     -> A ’-’ At                                => ’-’
     ->   ’+’ At
     ->   ’-’ At                                => ’neg’
     -> At ;
  */
void parser::A(){
	if (PARSERLOGS) printf ("Proc A next token = %s\n", nextToken->tokValue.c_str());
	string treeStr;
	if (nextToken->tokValue == "+"){
		read("+");
		At();
	} else if (nextToken->tokValue == "-"){
		read("-");
		At();
		buildTree ("neg", 1, treeNode::NEG);
	} else {
		At();
	}
	while (nextToken->tokValue == "+" || nextToken->tokValue == "-"){
		if (nextToken->tokValue == "+"){
			read("+");
			treeStr = "+";
		} else {
			read("-");
			treeStr = "-";
		}
		At();
		buildTree(treeStr, 2, treeStr == "+" ? treeNode::ADD: treeNode::SUBTRACT);
	}
}

/* At   -> At ’*’ Af                               => ’*’
     -> At ’/’ Af                               => ’/’
     -> Af ;
  */
void parser::At(){
	if (PARSERLOGS) printf ("Proc At next token = %s\n", nextToken->tokValue.c_str());
	string treeStr;
	Af();
	while(nextToken->tokValue == "*" || nextToken->tokValue == "/"){
		if (nextToken->tokValue == "*"){
			read("*");
			treeStr = "*";
		} else {
			read("/");
			treeStr = "/";
		}
		Af();
		buildTree(treeStr, 2, treeStr == "*" ? treeNode::MULTIPLY: treeNode::DIVIDE);
	}
}

/* Af   -> Ap ’**’ Af                              => ’**’
     -> Ap ;
  */
void parser::Af(){
	if (PARSERLOGS) printf ("Proc Af next token = %s\n", nextToken->tokValue.c_str());
	Ap();
	if (nextToken->tokValue == "**"){
		read("**");
		Af();
		buildTree("**", 2, treeNode::EXPONENTIAL);
	}
}

/* Ap   -> Ap ’@’ ’<IDENTIFIER>’ R                 => ’@’
     -> R ;
  */
void parser::Ap(){
	if (PARSERLOGS) printf ("Proc Ap next token = %s\n", nextToken->tokValue.c_str());
	R();
	while (nextToken->tokValue == "@"){
		read("@");
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
		R();
		buildTree("@", 3, treeNode::AT);
	}
}

/* R    -> R Rn                                    => ’gamma’
     -> Rn ;
*/
void parser::R(){
	if (PARSERLOGS) printf ("Proc R next token = %s\n", nextToken->tokValue.c_str());
	Rn();
	while ( (TOK_IDENTIFIER == nextToken->tokType  || TOK_INTEGER == nextToken->tokType|| TOK_STRING == nextToken->tokType
			||"(" == nextToken->tokValue || "false" == nextToken->tokValue
			|| "true" == nextToken->tokValue || "nil" == nextToken->tokValue || "dummy" == nextToken->tokValue) && !isKeyword(nextToken->tokValue)){
		Rn();
		buildTree("gamma", 2, treeNode::GAMMA);
	}
}

/* Rn   -> ’<IDENTIFIER>’
     -> ’<INTEGER>’
     -> ’<STRING>’
     -> ’true’                                  => ’true’
     -> ’false’                                 => ’false’
     -> ’nil’                                   => ’nil’
     -> ’(’ E ’)’
     -> ’dummy’                                 => ’dummy’ ;
*/
void parser::Rn(){
	if (PARSERLOGS) printf ("Proc Rn next token = %s\n", nextToken->tokValue.c_str());
	if("(" == nextToken->tokValue){
		read("(");
		E();
		read(")");
	}
	else if(TOK_IDENTIFIER == nextToken->tokType || TOK_INTEGER == nextToken->tokType || TOK_STRING == nextToken->tokType){
		if("true" == nextToken->tokValue){
			read("true");
			buildTree("<true>", 0, treeNode::TRUE);
		}
		else if("false" == nextToken->tokValue){
			read("false");
			buildTree("<false>", 0, treeNode::FALSE);
		}
		else if("nil" == nextToken->tokValue){
			read("nil");
			buildTree("<nil>", 0, treeNode::NIL);
		}
		else if("dummy" == nextToken->tokValue){
			read("dummy");
			buildTree("<dummy>",0, treeNode::DUMMY);
		}
		else if(TOK_IDENTIFIER == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			if (PARSERLOGS) printf ("next Token = %s\n", nextToken->tokValue.c_str());
		}
		else if(TOK_STRING == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::STRING);
			read(nextToken->tokValue);
		}
		else if(TOK_INTEGER == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::INTEGER);
			read(nextToken->tokValue);
		}
	}
}

/* D    -> Da ’within’ D                           => ’within’
     -> Da ;
  */
void parser::D(){
	if (PARSERLOGS) printf ("Proc D next token = %s\n", nextToken->tokValue.c_str());
	Da();
	if (nextToken->tokValue == "within"){
		read("within");
		D();
		buildTree("within", 2, treeNode::WITHIN);
	}
}
/*     Da   -> Dr ( ’and’ Dr )+                        => ’and’
         -> Dr ;
  */
void parser::Da(){
	if (PARSERLOGS) printf ("Proc Da next token = %s\n", nextToken->tokValue.c_str());
	int n = 0;
	Dr();
	while (nextToken->tokValue == "and"){
		read("and");
		Dr();
		n++;
	}
	if (n >0)
		buildTree("and", n+1, treeNode::AND);
}

/*      Dr   -> ’rec’ Db                                => ’rec’
         -> Db ;
  */
void parser::Dr(){
	if (PARSERLOGS) printf ("Proc Dr next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "rec"){
		read("rec");
		Db();
		buildTree("rec", 1, treeNode::REC);
	} else {
		Db();
	}
}

/* Db   -> Vl ’=’ E                              => ’=’
     -> ’<IDENTIFIER>’ Vb+ ’=’ E              => ’fcn_form’
     -> ’(’ D ’)’ ;
  */
void parser::Db(){
	if (PARSERLOGS) printf ("Proc Db next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "("){
		read("(");
		D();
		read(")");
	} else if (nextToken->tokType == TOK_IDENTIFIER){
	    //Since identifier type is common here, read it here now and consider it for build tree later.
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
		if (nextToken->tokValue == "," || nextToken->tokValue == "="){
			Vl();
			read("=");
			E();
			buildTree ("=", 2, treeNode::BINDING);
		} else {
			int n = 0;
			do {
				Vb();
				n++;
			}while (nextToken->tokValue == "(" || nextToken->tokType == TOK_IDENTIFIER);
			read("=");
			E();
			buildTree("function_form", n+2, treeNode::FCN_FORM); //The identifier at the start of this function is included here as n + 2
		}
	}
}

/*    Vb   -> ’<IDENTIFIER>’
         -> ’(’ Vl ’)’
         -> ’(’ ’)’                                 => ’()’;
  */
void parser::Vb(){
	if (PARSERLOGS) printf ("Proc Vb next token = %s\n", nextToken->tokValue.c_str());
	if(nextToken->tokType == TOK_IDENTIFIER){
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
	}else if (nextToken->tokValue == "("){
		read("(");
		if (nextToken->tokValue == ")"){
			read(")");
			buildTree("()", 0, treeNode::PARANTHESES);
		} else if (nextToken->tokType == TOK_IDENTIFIER){
		    //Before getting into Vl, an identifier must be read
		    //Vl expects its caller to do this.
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			Vl();
			read(")");
		}
	}
}

//    Vl   -> ’<IDENTIFIER>’ list ’,’                 => ’,’?;
void parser::Vl(){
	if (PARSERLOGS) printf ("Proc Vl next token = %s\n", nextToken->tokValue.c_str());
	int n = 0;
	while (nextToken->tokValue == ","){
		read(",");
		if (nextToken->tokType == TOK_IDENTIFIER){
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			n++;
		} else {
			printf ("ERROR In Vl()\n");
		}
	}
	//n+1 for the identifier that was read before Vl was called.
	if (n > 0)
		buildTree(",", n+1, treeNode::COMMA);
}
