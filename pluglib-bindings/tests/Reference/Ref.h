
#include <list>
#include <string>

enum e {A, B, C, D};


void ListInt(std::list<int>);
void RListInt(std::list<int>&);

void RInt(int& x);
void PInt(int* x);

void REnum(enum e& x);
void PEnum(enum e* x);

void RBool(bool& x);
void PBool(bool* x);

int RStr(std::string&);
int PStr(std::string*);
int CRStr(const std::string&);
int CPStr(const std::string*);

