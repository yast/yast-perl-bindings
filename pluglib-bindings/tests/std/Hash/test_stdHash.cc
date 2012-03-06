#include <iostream>
#include <iterator>
#include <cstdio>

#include <string>
#include <map>

using namespace std;


map<string,string> RStrStr(map<string,string> &l)
{
    printf("D: {");
    for(map<string,string>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%s=>%s, ", (i->first).c_str(), (i->second).c_str());
	(i->second)+=".ok";
    }
    printf("}\n");
    l["hu"]="he";
    return l;
}

void IntStr(map<int,string> l)
{
    printf("D: {");
    for(map<int,string>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%i=>%s, ", i->first, (i->second).c_str());
    }
    printf("}\n");
}

void IntBool(map<int,bool> l)
{
    printf("D: {");
    for(map<int,bool>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%i=>%i, ", i->first, i->second);
    }
    printf("}\n");
}

map<int,int> IntInt(map<int,int> l)
{
    printf("D: {");
    for(map<int,int>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%i=>%i, ", i->first, i->second);
	(i->second)++;
    }
    printf("}\n");
    l[8]=80;
}


void RIntInt(map<int,int> &l)
{
    printf("D: {");
    for(map<int,int>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%i=>%i, ", i->first, i->second);
	(i->second)--;
    }
    printf("}\n");
    l[9]=90;
}

