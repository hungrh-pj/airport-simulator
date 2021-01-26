// Airplane.hpp
#ifndef AIRPLANE_HPP // include guard
#define AIRPLANE_HPP

#include <cstdint>
#include "AirplaneState.hpp"

class Airplane
{
    std::uint32_t _id;
    std::uint32_t _fuel;
    std::uint32_t _fuel_consumption_rate;
    std::uint32_t _airport_answer_timeout;

    AirplaneState _state;

    void update_state(void);

public:
    Airplane(std::uint32_t id, std::uint32_t fuel, std::uint32_t fuel_cons_rate,
        std::uint32_t airport_answer_timeout);

    virtual AirplaneState get_state(void);

    virtual void take_off_authorized(void);
    virtual void take_off_denied(void);

    virtual void land_authorized(void);
    virtual void land_denied(void);
};

#endif /* AIRPLANE_HPP */