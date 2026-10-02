#ifndef CLASS_H
#define CLASS_H
#include<iostream>
#include<memory>
#include<string>
#include<vector>
#include<mysql/jdbc.h>
using namespace std;

class Delegate{
   public: 
   string name, country;
    int age, num, mun_attend, num1;
    vector<string> prev_committees;
    vector<string>awards;
    void input();
     };
sql::Connection* con = nullptr;
extern vector<Delegate> delegates;
#endif