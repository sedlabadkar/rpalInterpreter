#include "lexer.h"
#include "treeNode.h"

#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>

class Control{
public:
  void addCtrl(treeNode* node, int , string,  vector<string> *, Control*, int);
  void pretty_print();
  enum Type{
    ENV = 1 ,
    DELTA = 2 ,
    NAME = 3 ,
    LAMBDA = 4 ,
    GAMMA = 5 ,
    AUG = 6 ,
    BETA = 7 ,
    OR = 8 ,
    AND_LOGICAL = 9 ,
    NOT = 10 ,
    GR = 11 ,
    GE = 12 ,
    LS = 13 ,
    LE = 14 ,
    EQ = 15 ,
    NE = 16 ,
    ADD = 17 ,
    SUBTRACT = 18 ,
    NEG = 19 ,
    MULTIPLY = 20 ,
    DIVIDE = 21 ,
    EXP = 22 ,
    AT = 23 ,
    TRUE = 24 ,
    FALSE = 25 ,
    NIL = 26 ,
    DUMMY = 27 ,
    YSTAR = 28 ,
    ETA = 29 ,
    TAU = 30 ,
    STRING = 31 ,
    INTEGER = 32 ,
    TUPLE = 33
    };
  string toStr();
  Control( Control *cntrl);
  Control(Control::Type type, int index);
  Control(Control::Type type,vector<string> *variables, Control *del_ptr, int delta_index);
  Control(Control::Type type, int index, bool watever);
  Control(string var_value, Control::Type type );
  Control(Control::Type type, string value);
  Control();
  Control(Control::Type type);
  vector<Control *> *ctrlStruct;
  vector<string> variables;
  vector<Control *> ctrlTuples;
  int associatedENV;
  Type type;
  int index;
  string ctrlVal;
  Control *delta;

};
