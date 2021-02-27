#include "Airplane.hpp"

//  Airport is declared at header by forward declaration
#include "Airport.hpp"

Airplane::Airplane()
{
    _info = AirplaneInfo();
    _state = AirplaneState::unknown;
    _airport = nullptr;

    _is_waiting_take_off_authorization = false;
    _is_take_off_authorized = false;
    _is_waiting_landing_authorization = false;
    _is_landing_authorized = false;
}

Airplane::Airplane(AirplaneInfo& airplane_info)
{
    _info = airplane_info;
    _state = AirplaneState::stopped;
    _airport = nullptr;

    _is_waiting_take_off_authorization = false;
    _is_take_off_authorized = false;
    _is_waiting_landing_authorization = false;
    _is_landing_authorized = false;
}

void Airplane::set_state(AirplaneState state)
{
    _state = state;
}

AirplaneState Airplane::get_state(void)
{
    return _state;
}

void Airplane::set_airport(std::shared_ptr<Airport> airport)
{
    _airport = airport;
}

std::uint32_t Airplane::get_id(void)
{
    return _info._id;
}

std::uint32_t Airplane::get_fuel(void)
{
    return _info._fuel;
}

std::uint32_t Airplane::get_fuel_consumption_rate(void)
{
    return _info._fuel_consumption_rate;
}

std::shared_ptr<Airport> Airplane::get_airport(void)
{
    return _airport;
}

void Airplane::refuel(void)
{
    _info._fuel = _info._fuel_capacity;
}

void Airplane::take_off_authorized(void)
{
    _is_take_off_authorized = true;
}

void Airplane::take_off_denied(void)
{
    _is_take_off_authorized = false;
}

void Airplane::land_authorized(void)
{
    _is_landing_authorized = true;
}

void Airplane::land_denied(void)
{
    _is_landing_authorized = false;
}

bool Airplane::is_fuel_consumption_state(void)
{
    bool result = false;
    switch (_state)
    {
        case AirplaneState::taking_off:
        case AirplaneState::flying:
        case AirplaneState::asking_permission_to_land:
        case AirplaneState::landing:
        {
            result = true;
        }
        break;

        case AirplaneState::unknown:
        case AirplaneState::stopped:
        case AirplaneState::asking_permission_to_take_off:
        case AirplaneState::landed:
        case AirplaneState::crashed:
        default:
        {
            //  do nothing
        }
        break;
    }
    return result;
}

void Airplane::update_state(void)
{
    switch (_state)
    {
        case AirplaneState::stopped:
        {
            set_state(AirplaneState::asking_permission_to_take_off);
            _is_waiting_take_off_authorization = false;
        }
        break;

        case AirplaneState::asking_permission_to_take_off:
        {
            if (_is_waiting_take_off_authorization == false)
            {
                if (_airport != nullptr)
                {
                    _airport->permission_to_take_off(shared_from_this());
                    _is_waiting_take_off_authorization = true;
                    _info._airport_answer_time_unit_counter = 0;
                }
            }
            else
            {
                if (_is_take_off_authorized)
                {
                    set_state(AirplaneState::taking_off);
                    _info._taking_off_time_unit_counter = 0;
                }
                else
                {
                    if (_info._airport_answer_time_unit_counter >= _info._airport_answer_time_unit_limit)
                    {
                        _info._airport_answer_time_unit_counter = 0;
                        _is_waiting_take_off_authorization = false;
                    }
                    else
                    {
                        ++(_info._airport_answer_time_unit_counter);
                    }
                }
            }
        }
        break;

        case AirplaneState::taking_off:
        {
            if (_info._taking_off_time_unit_counter >= _info._taking_off_time_unit_limit)
            {
                set_state(AirplaneState::flying);
                _info._flying_time_unit_counter = 0;
            }
            else
            {
                ++(_info._taking_off_time_unit_counter);
            }
        }
        break;

        case AirplaneState::flying:
        {
            if (_info._flying_time_unit_counter >= _info._flying_time_unit_limit)
            {
                set_state(AirplaneState::asking_permission_to_land);
                _is_waiting_landing_authorization = false;
            }
            else
            {
                ++(_info._flying_time_unit_counter);
            }
        }
        break;

        case AirplaneState::asking_permission_to_land:
        {
            if (_is_waiting_landing_authorization == false)
            {
                if (_airport != nullptr)
                {
                    _airport->permission_to_land(shared_from_this());
                    _is_waiting_landing_authorization = true;
                    _info._airport_answer_time_unit_counter = 0;
                }
            }
            else
            {
                if (_is_landing_authorized)
                {
                    set_state(AirplaneState::landing);
                    _info._landing_time_unit_counter = 0;
                }
                else
                {
                    if (_info._airport_answer_time_unit_counter >= _info._airport_answer_time_unit_limit)
                    {
                        _info._airport_answer_time_unit_counter = 0;
                        _is_waiting_landing_authorization = false;
                    }
                    else
                    {
                        ++(_info._airport_answer_time_unit_counter);
                    }
                }
            }
        }
        break;

        case AirplaneState::landing:
        {
            if (_info._landing_time_unit_counter >= _info._landing_time_unit_limit)
            {
                set_state(AirplaneState::landed);
                _info._landed_time_unit_counter = 0;
                _is_waiting_landing_authorization = false;
            }
            else
            {
                ++(_info._landing_time_unit_counter);
            }
        }
        break;

        case AirplaneState::landed:
        {
            if (_info._landed_time_unit_counter >= _info._landed_time_unit_limit)
            {
                set_state(AirplaneState::stopped);
            }
            else
            {
                ++(_info._landed_time_unit_counter);
            }
        }
        break;

        case AirplaneState::unknown:
        case AirplaneState::crashed:
        default:
        {
            //  do nothing
        }
        break;
    }
}

void Airplane::update_time_unit(void)
{
    if (is_fuel_consumption_state())
    {
        std::int32_t fuel = (_info._fuel - _info._fuel_consumption_rate);
        if (fuel < 0)
        {
            set_state(AirplaneState::crashed);
            _info._fuel = 0;
        }
        else
        {
            _info._fuel -= _info._fuel_consumption_rate;
        }
    }
    else
    {
        //  do nothing - in a real world it has fuel consumption in all states but
        //this fact will be ignored it for now
    }

    update_state();
}

