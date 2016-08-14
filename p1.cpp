/*
 * p1.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: Sachin Edlabadkar
 */
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

ifstream source;
int option = 0x0000;
enum cmdOpts{
    OPT_L = 0x0001,
    OPT_NOOUT = 0x0010,
    OPT_ST = 0x0100,
    OPT_AST = 0x1000
};

bool isEvaluateProg = true;

void printOptionL(){
	string line;
	getline(source, line);
	while (source.good()){
		printf ("%s\n", line.c_str());
		getline(source, line);
	}
	source.clear();
	source.seekg(0, ios::beg);
}

void exec() {
	lexer* rpalLexer = new lexer(&source);
	parser* rpalParser = new parser(rpalLexer);
	if (option & OPT_L){
		printOptionL();
	}
	if(option & OPT_AST) {
		rpalParser->printAST();
	}
	if(option & OPT_ST) {
		rpalParser->printST();
	    //printf ("Option -st Not Implemented. Current Valid options are: -l -ast\n");
	}
	if(option & OPT_NOOUT){
		printf ("Option -noout Not Implemented. Current Valid options are: -l -ast\n");
	}
	if (isEvaluateProg){
	    //sprintf ("Evaluate program\n");
	    rpalParser->evaluateProg();
	}
}

int main (int argc, char *argv[]){
	int i = 1;
	//Usage: rpal <Switches; Can be more than 1> <Filename; Always the last>
	if (argc >= 2){
	    char* filename = argv[argc-1];
	    if (MAIN_DEBUG_LOGS) printf ("Filename = %s\n", filename);
	    source.open(filename);
	    if (!source.good()){
	        printf ("File \"%s\" not found\n", filename);
	        return 0;
	    }
	    //order of switches doesn't matter. More than one switch can be added. Not all are implemented in p1 though.
		while (i<argc - 1){
			if (strcmp(argv[i], "-ast") == 0){
			    option |= OPT_AST;
			    isEvaluateProg = false;
			} else if (strcmp(argv[i], "-st") == 0){
				option |= OPT_ST;
				isEvaluateProg = false;
			} else if (strcmp (argv[i], "-noout") == 0){
				option |= OPT_NOOUT;
				isEvaluateProg = false;
			} else if (strcmp(argv[i], "-l") == 0){
				option |= OPT_L;
				isEvaluateProg = false;
			} else {
				if (MAIN_DEBUG_LOGS) printf ("Invalid Option\n");
			}
			i++;
		}
		exec();
		source.close();
	} else {
		printf ("[rpal Compiler \n Author: Sachin Edlabadkar]\n\n");
		printf ("Usage: ./p1 [-l] [-ast] [-noout] [-st] <filename>\n\n");
	}
}
