#ifndef AIRPORT_HPP // include guard
#define AIRPORT_HPP

#include <memory>
#include <cstdint>

#include "Airplane.hpp"


class Airport
{
public:
    /**
     * Class Constructor
     */
    Airport();

    virtual void permission_to_take_off(std::shared_ptr<Airplane> airplane);
    virtual void permission_to_land(std::shared_ptr<Airplane> airplane);
};

#endif /* AIRPORT_HPP */