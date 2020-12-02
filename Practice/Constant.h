#include"Variable.h"
class Constant{
public:
    std::string val;
    Type type;
    Constant();
    Constant(Type _type,std::string _val);
};