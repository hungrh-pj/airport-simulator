#include "Airport.hpp"

Airport::Airport()
{
}

Airport::Airport(std::uint16_t& number_run_ways)
{
    _number_run_ways = number_run_ways;
}

// Airport::~Airport()
// {
// }

void Airport::permission_to_take_off(std::shared_ptr<Airplane> airplane)
{
    // (void)airplane;
    airplane->take_off_authorized();
}

void Airport::permission_to_land(std::shared_ptr<Airplane> airplane)
{
    // (void)airplane;
    airplane->land_authorized();
}
