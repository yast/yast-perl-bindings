
#include "Simple.h"

xyz* create_xyz()
{
    return new xyz;
}

void xyz::decX()
{
    x--;
}
