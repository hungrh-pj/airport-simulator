#ifndef AIRPORT_HPP // include guard
#define AIRPORT_HPP

#include <memory>
#include <cstdint>

#include "Airplane.hpp"


class Airport
{
public:
    /**
     * Default Class Constructor
     */
    Airport();

    /**
     * Class Constructor
     */
    Airport(std::uint16_t& number_run_ways);

    virtual void permission_to_take_off(std::shared_ptr<Airplane> airplane);
    virtual void permission_to_land(std::shared_ptr<Airplane> airplane);

private:
    std::uint16_t _number_run_ways;
};

#endif /* AIRPORT_HPP */