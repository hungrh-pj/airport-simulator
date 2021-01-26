#include "Airplane.hpp"

Airplane::Airplane(std::uint32_t id, std::uint32_t fuel, std::uint32_t fuel_cons_rate,
    std::uint32_t airport_answer_timeout)
{
    this->_id = id;
    this->_fuel = fuel;
    this->_fuel_consumption_rate = fuel_cons_rate;
    this->_airport_answer_timeout = airport_answer_timeout;
    this->_state = AIRPLANE_STOPPED;
}

AirplaneState Airplane::get_state(void)
{
    return this->_state;
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
