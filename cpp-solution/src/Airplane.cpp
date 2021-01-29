#include "Airplane.hpp"

Airplane::Airplane()
{
}

Airplane::Airplane(AirplaneInfo& airplane_info)
{
    this->_info = airplane_info;
    this->_state = AirplaneState::stopped;
}

// Airplane::~Airplane()
// {
// }

AirplaneState Airplane::get_state(void)
{
    return this->_state;
}

void Airplane::set_airport(std::shared_ptr<Airport> airport)
{
    this->_airport = airport;
}

std::uint32_t Airplane::get_id(void)
{
    return this->_info._id;
}

std::uint32_t Airplane::get_fuel(void)
{
    return this->_info._fuel;
}

void Airplane::take_off_authorized(void)
{
}

void Airplane::take_off_denied(void)
{
}

void Airplane::land_authorized(void)
{
}

void Airplane::land_denied(void)
{
}

void Airplane::update_time_unit(void)
{
}
