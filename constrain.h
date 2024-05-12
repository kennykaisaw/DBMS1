#ifndef CONSTRAIN_H
#define CONSTRAIN_H
#include"table.h"
#include<algorithm>
#include<string>
#include<iostream>
#include<vector>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;
bool evaluate(const std::string& expr, const std::vector<bool>& values);
//> < = in between..and or
template<class T>
class daYu
{
private:
    T thing;
public:
    daYu(const T& thing):thing(thing){}
    bool operator()(T compare){return compare>thing;}
};
template<class T>
class xiaoYu
{
private:
    T thing;
public:
    xiaoYu(const T& thing):thing(thing){}
    bool operator()(T compare){return compare<thing;}
};
template<class T>
class daYuDengYu
{
private:
    T thing;
public:
    daYuDengYu(const T& thing):thing(thing){}
    bool operator()(T compare){return compare>=thing;}
};
template<class T>
class xiaoYuDengYu
{
private:
    T thing;
public:
    xiaoYuDengYu(const T& thing):thing(thing){}
    bool operator()(T compare){return compare<thing;}
};
template<class T>
class dengYu
{
private:
    T thing;
public:
    dengYu(const T& thing):thing(thing){}
    bool operator()(T compare){return compare==thing;}
};
template<class T>
class between
{
private:
    T thing1;
    T thing2;
public:
    between(const T& thing1,const T& thing2):thing1(thing1),thing2(thing2){}
    bool operator()(T compare){return compare<=thing2&&compare>=thing1;}
};
template<class T>
class in
{
private:
    vector<T> list;
public:
    in(const vector<T>& list):list(list){}
    bool operator()(T compare)
    {
        for(unsigned int i=0;i<list.size();++i)
        {
            if(list[i]==compare) return true;
        }
        return false;
    }
};
template<class T>
class notIn
{
private:
    vector<T> list;
public:
    notIn(const vector<T>& list):list(list){}
    bool operator()(T compare)
    {
        for(unsigned int i=0;i<list.size();++i)
        {
            if(list[i]==compare) return false;
        }
        return true;
    }
};



#endif // CONSTRAIN_H
