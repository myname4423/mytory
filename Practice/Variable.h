#include"type.h"
#include<string>
class Variable
{
public:
    std::string name;
    void* val;
    Type type;
    bool unKnown;
    Variable(Type _type,void* _val,std::string _name);
    Variable(std::string _name);
    Variable();
};