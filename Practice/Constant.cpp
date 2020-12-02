#include"Constant.h"
#include<iostream>
Constant::Constant(Type _type,std::string _val){
    type=_type;
    val=_val;
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
Constant::Constant(){}