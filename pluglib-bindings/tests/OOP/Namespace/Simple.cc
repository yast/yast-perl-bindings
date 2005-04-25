
#include "Simple.h"

Complication::xyz* Complication::create_xyz()
{
    return new Complication::xyz;
}

void Complication::xyz::decX()
{
    x--;
}
