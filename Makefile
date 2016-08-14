all:	
	g++ -std=c++11 token.cpp TreeStandardizer.cpp lexer.cpp Control.cpp parser.cpp Environment.cpp CSEMachine.cpp p1.cpp -o p2
cl: 
	rm -f p1;
