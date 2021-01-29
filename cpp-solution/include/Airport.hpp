#ifndef AIRPORT_HPP // include guard
#define AIRPORT_HPP

// //  TODO: not worry about how manage object in memory by now
// #include <memory>
#include <cstdint>

// Forward Declaration class Airplane
class Airplane;

class Airport
{
public:
    /**
     * Class Constructor
     */
    Airport();

    //  TODO: rule of three / five?
    // virtual ~Airplane();

    virtual bool permission_to_take_off(Airplane& airplane);
    virtual bool permission_to_land(Airplane& airplane);
};

#endif /* AIRPORT_HPP */