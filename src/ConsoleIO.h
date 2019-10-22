#include <iostream>
#include <string>
#include "UnlimitedInteger.h"
#include "list/List.h"
#include <string>
#include <conio.h>
#include <fstream>
#include <map>
#include <functional>
using namespace std;

#ifndef CONSOLEIO
#define CONSOLEIO

class ConsoleIO
{
private:
    map<char,function<string(const UnlimitedInteger &,const UnlimitedInteger &)> > funcMap;
    //加载运算符函数(为了去掉compute()里的switch-case，编了这种东西,start()函数里的switch-case我懒得去了)
    map<char,function<string(const UnlimitedInteger &,const UnlimitedInteger &)> > prepareOperators();
    //按照表达式计算(不支持多个运算符)
    string compute(string expression);
public:
    void start();
    ConsoleIO(){
        funcMap = prepareOperators();
    }
};
#endif