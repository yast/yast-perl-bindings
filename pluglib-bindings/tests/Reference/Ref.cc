
#include <list>
#include <string>

#include <stdio.h>

#include "Ref.h"

using namespace std;

void ListInt(list<int> l)
{
}

void RListInt(list<int> &l)
{
    for(list<int>::iterator i = l.begin(); i!=l.end(); i++) {
	(*i)++;
    }
}

void RInt(int &l)
{
    l=l*2;
}

void PInt(int *l)
{
    *l=*l+2;
}

void REnum(e &l)
{
    l=A;
}

void PEnum(e *l)
{
    *l=B;
}

void RBool(bool &l)
{
    l=true;
}

void PBool(bool *l)
{
    *l=false;
}

int RStr(string &l)
{
    l += ".A";
    return 8;
}

int PStr(string *l)
{
    *l += ".B";
    return 8;
}

int CRStr(const string &l)
{
printf("D: crs=%s\n", l.c_str());
    return 8;
}

int CPStr(const string *l)
{
printf("D: cps=%s\n", l->c_str());
    return 8;
}
