#include "Airport.hpp"

Airport::Airport()
{
}

// Airport::~Airport()
// {
// }

bool Airport::permission_to_take_off(Airplane& airplane)
{
    (void)airplane;
    return false;
}

bool Airport::permission_to_land(Airplane& airplane)
{
    (void)airplane;
    return false;
}
