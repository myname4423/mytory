#include<iostream>
#include<unordered_map>
#include<string>
#include<stdio.h>
#include<vector>
#include<queue>
#include"Constant.h"
#include"Tuple.h"
#include<stack>
using namespace std;
#define shamt 60

//标识符	36  整数	37  字符常数	38
//0-><>  1->() 2->[] 3->'' 4->/* */
int match[5]={0};
unordered_map<string,int> map{{"and",1},{"output",21},{"*",41},{"array",2},{"procedure",22},{"*/",42},{"begin",3},{"program",23},{"+",43},{"bool",4},{"read",24},{",",44},{"call",5},{"real",25},{"-",45},{"case",6},{"repeat",26},{".",46},{"char",7},{"set",27},{"..",47},{"constant",8},{"stop",28},{"/",48},{"dim",9},{"then",29},{"/*",49},{"do",10},{"to",30},{":",50},{"else",11},{"true",31},{":=",51},{"end",12},{"until",32},{";",52},{"false",13},{"var",33},{"<",53},{"for",14},{"while",34},{"<=",54},{"if",15},{"write",35},{"<>",55},{"input",16},{"=",56},{"integer",17},{">",57},{"not",18},{">=",58},{"of",19},{"(",39},{"[",59},{"or",20},{")",40},{"]",60}};
unordered_map<string,int> ider;
string buffer="";
int kind=0;
int identifier_num=0;

//实验二的数据结构
/*
输入流：输入的是int型的，即非终结符的编号；
其中负数的值代表标识符，其绝对值对应符号表中该非终结符的索引；
大于shamt的正值代表常数，其与shamt的差值对应常数表中该常数的索引
*/
queue<int> words;
//常量区(包括整数常量和字符常量)
vector<Constant> constList{Constant()};
int currCon=0;
//变量区
vector<Variable> varList{Variable("null")};
int currVar=0;
//四元组
vector<Tuple> result;
int curr=0;
//算术栈和符号栈
stack<int> cal_stack;
stack<Op> op_stack;
//变量的数目，大于该值的是临时变量
int var_num=0;


/*词法分析函数*/
//词法分析打印输出
void print_1(int s,int n);
//处理完一个词后
void show();
//输出作者信息
void author();
//词法分析
void word_analysis();

/*语法分析函数*/
//语法分析
void sentence_analysis();


int main(int argc, char *argv[]){
    author();
    word_analysis();
    sentence_analysis();
    return 0;
}

//输出作者信息
void author(){
    string author="Yunbo Zhang";
    string stu_no="201830291488";
    string stu_class="CS 2";
    cout<<"author:"<<author<<'\t'<<"number:"<<stu_no<<'\t'<<"class:"<<stu_class<<endl;
}
//词法分析打印输出
void print_1(int s,int n){
    if(n<0)cout<<"("<<s<<",-)"<<endl;
    else cout<<"("<<s<<","<<n<<")"<<endl;
}
//词法分析
void word_analysis(){
    //打开文件
    string filename;
    cout<<"The number of file you want to open is "<<endl;
    //cin>>filename;
    filename="2";
    filename="program_"+filename+".zyb";
    auto ptr=freopen(filename.c_str(), "r", stdin);
    if(ptr==NULL){
        cout<<"the file NUMBER you give not exists!"<<endl;
        exit(0);
    }
    //开始词法分析
    char token;
    while ((token = cin.get()) != EOF){
        if(match[4]>0){
            if(token=='*'&&cin.peek()=='/'){
                cin>>token;
                match[4]--;
            }
            if(token=='\n'){
                cout<<"\\n shoudn't appear in comment!"<<endl;
                exit(0);
            }
        }
        else if(match[3]>0){
            buffer+=token;
            if(cin.peek()=='\''){
                cin>>token;
                match[3]--;
                if(match[3]==0)show();
            }
            
        }
        else if(token=='+'||token=='-'||token=='*'||token==';'||token==','||token=='='||token==' '||token=='\n'){
            show();
            if(token!=' '&&token!='\n')buffer+=token;
            show();
        }
        else if(token=='\''){
            match[3]++;
            kind=38;
        }
        else if(token=='/'){
            if(cin.peek()=='*')match[4]++;
            else{
                show();
                buffer+=token;
                show();
            }
        }
        else if(token=='('||token==')'||token=='['||token==']'){
            show();
            buffer+=token;
            switch (token)
            {
            case '(':
                match[1]++;
                break;
            case ')':
                match[1]--;
                break;
            case '[':
                match[2]++;
                break;
            case ']':
                match[2]--;
            }
            show();
        }
        else if(token==':'||token=='.'){
            show();
            buffer+=token;
            if(token==':'&&cin.peek()=='='||token=='.'&&cin.peek()=='.'){
                cin>>token;
                buffer+=token;
            }
            show();
        }
        else if(token=='<'||token=='>'){
            show();
            buffer+=token;
            if(cin.peek()=='='){
                cin>>token;
                buffer+=token;
            }
            else{
                if(token=='<'){
                    if(cin.peek()=='>'){
                        cin>>token;
                        buffer+=token;
                    }
                    else match[0]++;
                }
                else match[0]--;
            }
            show();
        }
        else{
            if(kind==0){
                if(isdigit(token))kind=37;
                else if(isalpha(token))kind=36;
                else{
                    cout<<"invaild linput"<<endl;
                    exit(0);
                }
            }
            if(kind==36)buffer+=token;
            else{
                if(isdigit(token))buffer+=token;
                else{
                    cout<<"invalid identifier!"<<endl;
                    exit(0);
                }
            }
        }
    }

    for(int i=0;i<4;i++){
        if(match[i]!=0){
            cout<<"wrong pairs"<<endl;
            exit(0);
        }
    }
    
}
//处理完一个词后
void show(){
    if(buffer.empty())return;
    auto temp=map.find(buffer);
    if(temp!=map.end()){
        print_1(temp->second,-1);
        words.push(temp->second);
    } 
    else if(kind==37){
        print_1(kind,++identifier_num);
        words.push(++currCon+shamt);
        constList.emplace_back(Integer,buffer);
    }
    else if(kind==38){
        print_1(kind,++identifier_num);
        words.push(++currCon+shamt);
        constList.emplace_back(Char,buffer);
    }
    else if(kind==36){
        temp=ider.find(buffer);
        if(temp!=ider.end()){
            print_1(kind,temp->second);
            words.push(-1*temp->second);
        }
        else{
            ider[buffer]=++identifier_num;
            print_1(kind,identifier_num);
            words.push(-1*identifier_num);
            varList.emplace_back(buffer);
        }
    }
    else{
        cout<<"invaild dinput!"<<endl;
        exit(0);
    }
    buffer.clear();
    kind=0;
}

//对于四元组(op,a,b,c)
//若op为:=或者运算符号，则a,b,c均为输入（含负）
//注意判断为常数是要进一步判断为常整数！！！constList[token-shamt].type==Integer;

void sentence_analysis(){
    //初始化
    currCon=currVar=curr=0;
    var_num=varList.size();
}
void error(){
    cout<<"wrong happened!"<<endl;
    exit(0);
}
int Exp(int token){
    //标识符、整数、(、-
    if(token<0||token>shamt||token==39||token==45){
        int temp=Term(token);
        cal_stack.push(temp);
        temp=Exp_2(token);
        //没有下一个项了，因此将当前栈中唯一一个弹出
        if(temp==0){
            temp=cal_stack.top();
            cal_stack.pop();
        }
        cout<<"stack empty is:"<<boolalpha<<cal_stack.empty()<<endl;
        return temp;
    }
    else error();
}
int Exp_2(int token){
    //token为+ or -
    if(token==43||token==45){
        //记录运算符
        if(token==43)op_stack.push(Add);
        else op_stack.push(Minus);
        //getnext(token)
        token=words.front();
        words.pop();

        int temp=Term(token);
        cal_stack.push(temp);
        if(cal_stack.size()>=2){
            int a=cal_stack.top();
            cal_stack.pop();
            int b=cal_stack.top();
            cal_stack.pop();
            Op op=op_stack.top();
            op_stack.pop();
            varList.emplace_back();
            int c=1-varList.size();
            result.emplace_back(op,a,b,c);
            cal_stack.push(c);
        }
        temp=Exp_2(token);
        if(temp==0){
            temp=cal_stack.top();
            cal_stack.pop();
        }
        cout<<"stack empty is:"<<boolalpha<<cal_stack.empty()<<endl;
        return temp;
    }
    //token 为 )
    else if(token==40){
        return 0;
    }
    //token 为# 这里先用;代替，后面再改！！！
    else if(token==52){
        //此时已经结束，因此返回0代表空
        return 0;
    }
    else error();
}
int Term(int token){
    //token 为标识符、常数、(、-
    if(token<0||token>shamt||token==39||token==45){
        int temp=Factor(token);
        cal_stack.push(temp);
        temp=Term_2(token);
        if(temp==0){
            temp=cal_stack.top();
            cal_stack.pop();
        }
        cout<<"stack size is:"<<boolalpha<<cal_stack.size()<<endl;
        return temp;
    }
    else error();
}
int Term_2(int token){
    //token为* or /
    if(token==41||token==48){
        //记录运算符
        if(token==41)op_stack.push(Mut);
        else op_stack.push(Div);
        //getnext(token)
        token=words.front();
        words.pop();

        int temp=Factor(token);
        cal_stack.push(temp);
        if(cal_stack.size()>=2){
            int a=cal_stack.top();
            cal_stack.pop();
            int b=cal_stack.top();
            cal_stack.pop();
            Op op=op_stack.top();
            op_stack.pop();
            varList.emplace_back();
            int c=1-varList.size();
            result.emplace_back(op,a,b,c);
            cal_stack.push(c);
        }
        temp=Term_2(token);
        if(temp==0){
            temp=cal_stack.top();
            cal_stack.pop();
        }
        cout<<"stack size is:"<<boolalpha<<cal_stack.size()<<endl;
        return temp;
    }
    else if(token==43||token==45||token==40||token==52){
        return 0;
    }
    else error();
}
int Factor(int token){
    if(token<0){

    }
    else if(token>shamt){

    }
    //token为(
    else if(token==39){

    }
    //token为-
    else if(token==45){

    }
    else error();
}


void print_2(){
    string op[]={"+","-","*","/"};
    for(auto tuple : result){
        auto temp=tuple.op;
        string a,b,c;
        switch (temp){
        case Add:
        case Minus:
        case Mut:
        case Div:
            a=to_string(tuple.a);
            b=to_string(tuple.b);
            c=to_string(tuple.c);
            break;
        
        default:
            break;
        }
        cout<<"("<<op[temp]<<","<<a<<","<<b<<","<<c<<")"<<endl;
    }
}


