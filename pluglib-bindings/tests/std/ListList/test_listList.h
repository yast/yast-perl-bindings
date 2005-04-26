
#include <list>
#include <string>

std::list<std::list<std::string> > RListListString(std::list<std::list<std::string> > &x);

std::list<std::list<int> > ListListInt(std::list<std::list<int> > x);
void RListListInt(std::list<std::list<int> >& x);
void PListListInt(std::list<std::list<int> >* x);
