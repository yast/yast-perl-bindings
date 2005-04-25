
#include <list>
#include <string>

void ListInt(std::list<int>);
void RListInt(std::list<int>&);

void RInt(int& REFERENCE);
void PInt(int* REFERENCE);

int RStr(std::string&);
int PStr(std::string*);
int CRStr(const std::string&);
int CPStr(const std::string*);

