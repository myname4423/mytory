#include "Variable.h"
#include<iostream>
Variable::Variable(Type _type,void* _val,std::string _name){
    unKnown=false;
    name=_name;
    type=_type;
    switch (type){
    case Integer:
    case Bool:
    case Char:
        val=_val;
        break;
    default:
        std::cout<<"wrong type!"<<std::endl;
    }
}
Variable::Variable(std::string _name){
    name=_name;
    unKnown=true;
}
Variable::Variable(){}


