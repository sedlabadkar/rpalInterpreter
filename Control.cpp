    #include "Control.h"

    string Control::toStr()
    {
        string str ;
        switch(type)
        {
        case DELTA :
            str = "<D" + std::to_string(index) + ">" ;
            return str;
        case NAME :
            return variables.at(0);//.front() ; //not sure y front() is not working
        case LAMBDA :
            str = "[lambda closure: " ;
            for( int i = 0 ; i < variables.size() ; i++ )
                str += variables.at(i) + ": " ;
            str += to_string(index) + "]" ;
            return str ;
        case GAMMA :
            return "Gamma" ;
        case AUG :
          return "AUG" ;
        case BETA :
            return "BETA" ;
        case OR :
            return "OR" ;
        case AND_LOGICAL :
          return "AND" ;
        case NOT :
          return "NOT" ;
        case GR :
            return ">" ;
        case GE :
            return ">=" ;
        case LS :
            return "<" ;
        case LE :
            return "<=" ;
        case EQ :
            return "=" ;
        case NE :
            return "!=" ;
        case ADD :
            return "+" ;
        case SUBTRACT :
            return "-" ;
        case NEG :
            return "NEG" ;
        case MULTIPLY :
            return "*" ;
        case DIVIDE :
            return "/" ;
        case EXP :
            return "**" ;
        case AT :
            return "@" ;
        case TRUE :
            return "true" ;
        case FALSE :
            return "false" ;
        case NIL :
            return "nil" ;
        case DUMMY :
            return "dummy" ;
        case YSTAR :
            return "Y" ;
        case TAU :
            str = "<T" + to_string(index) + ">"  ;
            return str;
        case STRING :
            return ctrlVal +"";
        case INTEGER :
            str = ctrlVal + "" ;
            return str;
        case ENV :
            str = "e" + to_string(index) ;
            return str;
        case ETA :
            str = "<ETA," + to_string(index) ;
            for( int i = 0 ; i < variables.size() ; i++ )
                str += "," + variables.at(i) ;
            str += ">" ;
            return str ;
        case TUPLE :
            str = "(" ;
            for( int i = 0 ; i < ctrlTuples.size() ; i++ )
            {
                str += ctrlTuples.at(i)->toStr() ;
                if( i != ctrlTuples.size()-1 )
                    str += ", " ;
            }
            str += ")" ;
            return str ;
        default :
            return "unknown" ;
        }
    }

  Control::Control(Control *cntrl)
  {
    index = cntrl->index;
    ctrlVal = cntrl->ctrlVal ;
    type = cntrl->type ;
    associatedENV = cntrl->associatedENV;
    if( !cntrl->variables.empty() ){
      for(int i=0;i<cntrl->variables.size();i++)
    variables.push_back(cntrl->variables.at(i)) ;
    }
    if( !cntrl->ctrlTuples.empty() ){
      for(int i=0;i<cntrl->ctrlTuples.size();i++)
    ctrlTuples.push_back(cntrl->ctrlTuples.at(i)) ;
    }
  }

  Control::Control(Control::Type type, int index){
    if(Control::DELTA != type){
      printf ("Control::DELTA 's constructor called for : %d", type);
    };
    this->type = type;
    this->index = index;
    ctrlStruct = new vector<Control *>();
  }

  Control::Control(Control::Type type,vector<string> *vars, Control *del_ptr, int deltaindex){
    this->type = type;//Must be LAMBDA
    index = deltaindex;
    if(vars != NULL){
      for(int i=0;i<vars->size();i++){
    variables.push_back(vars->at(i));
      }
    }
  }

  Control::Control(Control::Type type, int index, bool watever){
    if(type != Control::TAU && type != Control::ENV){
      printf("Control::TAU 's constructor called for : %d" , type );
    }
    this->type = type;
    this->index = index;
  }

  Control::Control(string var_value, Control::Type type ){
    this->type = type;
    variables.push_back(var_value);
  }

  Control::Control(Control::Type type, string value){
    this->type = type;
    ctrlVal = value;
  }

  Control::Control(){
  }

  Control::Control(Control::Type type) {
      this->type = type;
  }

void Control::pretty_print(){
  if(type!=Control::DELTA){
    printf("pretty_print called on non delta node, Not a delta node, cannot pretty print");
  };
  cout << toStr() << " ";
  for(int i=0; i< ctrlStruct->size(); i++){
      cout << ctrlStruct->at(i)->toStr() << " " ;
  };
  cout << endl;
}

void Control::addCtrl(treeNode* node, int type, string value, vector<string> *variables, Control* del_ptr, int deltas_size){
  int tau_count;
  Control *temp = NULL;
  switch(node->type){
  case treeNode::LAMBDA:
    temp = new Control(Control::LAMBDA, variables, del_ptr, deltas_size-1 );
    break;
  case treeNode::INTEGER:
      //Remove <ID: and > from the node string before storing it
    temp = new Control(Control::INTEGER, node->nodeString);
    break;
  case treeNode::MULTIPLY:
    temp = new Control(Control::MULTIPLY, node->nodeString);
    break;
  case treeNode::ADD:
    temp = new Control(Control::ADD, node->nodeString);
    break;
  case treeNode::SUBTRACT:
    temp = new Control(Control::SUBTRACT, node->nodeString);
    break;
  case treeNode::DIVIDE:
    temp = new Control(Control::DIVIDE, node->nodeString);
    break;
  case treeNode::GAMMA:
    temp = new Control(Control::GAMMA, node->nodeString);
    break;
  case treeNode::IDENTIFIER:
    temp = new Control(node->nodeString, Control::NAME);
    break;
  case treeNode::STRING:
      //Remove <ID: and > from the node string before storing it
    temp = new Control(Control::STRING, node->nodeString.substr(1, node->nodeString.length() - 2));
    break;
  case treeNode::TAU:
    if(variables!=NULL)
      tau_count = variables->size();
    else
      cout << "TAU add_control NULL variables sent!" << endl;
    temp = new Control(Control::TAU, tau_count, false);
    break;
  case treeNode::AUG:
    temp = new Control(Control::AUG);
    break;
  case treeNode::NIL:
    temp = new Control(Control::NIL);
    break;
  case treeNode::YSTAR:
    temp = new Control(Control::YSTAR);
    break;
  case treeNode::AND_LOGICAL:
    temp = new Control(Control::AND_LOGICAL);
    break;
  case treeNode::OR:
    temp = new Control(Control::OR);
    break;
  case treeNode::NE:
    temp = new Control(Control::NE);
    break;
  case treeNode::EQ:
    temp = new Control(Control::EQ);
    break;
  case treeNode::LS:
    temp = new Control(Control::LS);
    break;
  case treeNode::LE:
    temp = new Control(Control::LE);
    break;
  case treeNode::GR:
    temp = new Control(Control::GR);
    break;
  case treeNode::GE:
    temp = new Control(Control::GE);
    break;
  case treeNode::NEG:
    temp = new Control(Control::NEG);
    break;
  case treeNode::FALSE:
    temp = new Control(Control::FALSE);
    break;
  case treeNode::TRUE:
    temp = new Control(Control::TRUE);
    break;
  case treeNode::NOT:
    temp = new Control(Control::NOT);
    break;
  case treeNode::DUMMY:
    temp = new Control(Control::DUMMY);
    break;
  default:
    printf ("This treenode node handled. val: %s type: %d", value.c_str(), type);
    break;
  };
  ctrlStruct->push_back(temp);
}


