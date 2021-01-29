#ifndef AIRPLANE_HPP // include guard
#define AIRPLANE_HPP

#include <memory>
#include <cstdint>
#include "AirplaneState.hpp"

// Forward Declaration class Airport
class Airport;

struct AirplaneInfo
{
    /**
     * A public variable
     * Airplane ID
     */
    std::uint32_t _id;

    /**
     * A public variable
     * How many fuel the Airplane has (in liters)
     */
    std::uint32_t _fuel;

    /**
     * A public variable
     * How many fuel the Airplane consume in a time unit. To keep it simple, the consumption rate
     * is used only when the airplane is flying.
     *
     * Obs: in the real world if the airplane is stopped asking permission, the fuel is being
     * consumed (it is smaller than when the airplane is flying but it has a consume)
     */
    std::uint32_t _fuel_consumption_rate;

    /**
     * A public variable
     * For how long airplane will wait the answer from Airport
     */
    std::uint32_t _airport_answer_timeout;

    /**
     * Default Constructor
     */
    AirplaneInfo()
    {
        this->_id = 0;
        this->_fuel = 0;
        this->_fuel_consumption_rate = 0;
        this->_airport_answer_timeout = 0;
    }

    /**
     * Copy Constructor
     */
    AirplaneInfo(const AirplaneInfo& other)
    {
        this->_id = other._id;
        this->_fuel = other._fuel;
        this->_fuel_consumption_rate = other._fuel_consumption_rate;
        this->_airport_answer_timeout = other._airport_answer_timeout;
    }

    /**
     * Move Constructor
     */
    AirplaneInfo(AirplaneInfo&& other)
    {
        this->_id = other._id;
        this->_fuel = other._fuel;
        this->_fuel_consumption_rate = other._fuel_consumption_rate;
        this->_airport_answer_timeout = other._airport_answer_timeout;

        other._id = 0;
        other._fuel = 0;
        other._fuel_consumption_rate = 0;
        other._airport_answer_timeout = 0;
    }

    /**
     * Destructor
     */
    virtual ~AirplaneInfo()
    {
    }

    /**
     * Move assignment
     */
    AirplaneInfo& operator=(AirplaneInfo&& other)
    {
        this->_id = other._id;
        this->_fuel = other._fuel;
        this->_fuel_consumption_rate = other._fuel_consumption_rate;
        this->_airport_answer_timeout = other._airport_answer_timeout;

        other._id = 0;
        other._fuel = 0;
        other._fuel_consumption_rate = 0;
        other._airport_answer_timeout = 0;

        return *this;
    }

    /**
     * Copy assignment
     */
    AirplaneInfo& operator=(AirplaneInfo& other)
    {
        this->_id = other._id;
        this->_fuel = other._fuel;
        this->_fuel_consumption_rate = other._fuel_consumption_rate;
        this->_airport_answer_timeout = other._airport_answer_timeout;
        return *this;
    }
};

class Airplane
{
    AirplaneInfo _info;
    AirplaneState _state;

    // virtual void update_state(void);

    //  TODO: create Factory and instance Airplane from there!
    //  for now to keep it simple, using shared_ptr()
    std::shared_ptr<Airport> _airport;

public:
    /**
     * Class Constructor
     */
    Airplane();

    /**
     * Class Constructor
     * @param airplane_info information about airplane provided by AirplanInfo structure
     */
    Airplane(AirplaneInfo& airplane_info);

    //  TODO: rule of five!
    // virtual ~Airplane();

    /**
     * A function which return the Airplane's state
     * @return Airplane state
     */
    virtual AirplaneState get_state(void);

    virtual std::uint32_t get_id(void);
    virtual std::uint32_t get_fuel(void);

    /**
     * A function which set the airport this airplane will contact
     * @param airport airport which this airplane will contact
     */
    virtual void set_airport(std::shared_ptr<Airport> airport);

    /**
     * A function which update the time unit of Airplane, this is the input function to Airplane
     * update its fuel and check if it needs to change the state. Example: if the time unit is
     * seconds, this function should be called once in 1 second.
     */
    virtual void update_time_unit(void);

    virtual void take_off_authorized(void);
    virtual void take_off_denied(void);

    virtual void land_authorized(void);
    virtual void land_denied(void);
};

#endif /* AIRPLANE_HPP */