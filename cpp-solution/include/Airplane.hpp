#ifndef AIRPLANE_HPP // include guard
#define AIRPLANE_HPP

// Forward Declaration class Airport
class Airport;

#include <memory>
#include <cstdint>
#include "AirplaneState.hpp"

/**
 * The rule of five was follow becaus it is necessary to implemente the assignment function
 */
struct AirplaneInfo
{
    /**
     * A public variable
     * Airplane ID
     */
    std::uint32_t _id;

    /**
     * A public variable
     * How many fuel the Airplane has now (in liters)
     */
    std::uint32_t _fuel;

    /**
     * A public variable
     * How many fuel the Airplane is capable to carry (in liters)
     */
    std::uint32_t _fuel_capacity;

    /**
     * A public variable
     * How many fuel the Airplane consume in a time unit. To keep it simple, the consumption rate
     * is used only when the airplane is moving.
     *
     * Obs: in the real world if the airplane is stopped asking permission, the fuel is being
     * consumed (the consumption rate is lower than flying/moving consumption rate
     * but it has a consume)
     */
    std::uint32_t _fuel_consumption_rate;

    std::uint32_t _airport_answer_time_unit_counter;

    /**
     * A public variable
     * For how long airplane will wait the answer from Airport
     */
    std::uint32_t _airport_answer_time_unit_limit;

    std::uint32_t _taking_off_time_unit_counter;
    std::uint32_t _taking_off_time_unit_limit;

    std::uint32_t _flying_time_unit_counter;
    std::uint32_t _flying_time_unit_limit;

    std::uint32_t _landing_time_unit_counter;
    std::uint32_t _landing_time_unit_limit;

    std::uint32_t _landed_time_unit_counter;
    std::uint32_t _landed_time_unit_limit;

    /**
     * Default Constructor
     */
    AirplaneInfo()
    {
        reset();
    }

    /**
     * Copy Constructor
     * @param other object which info are copy from
     */
    AirplaneInfo(const AirplaneInfo& other)
    {
        update_members(other);
    }

    /**
     * Move Constructor
     * @param other object which info are move from
     */
    AirplaneInfo(AirplaneInfo&& other)
    {
        update_members(other);
        other.reset();
    }

    /**
     * Destructor
     */
    virtual ~AirplaneInfo()
    {
        reset();
    }

    /**
     * Copy assignment
     * @param other object which info are copy from
     */
    AirplaneInfo& operator=(AirplaneInfo& other)
    {
        update_members(other);
        return *this;
    }

    /**
     * Move assignment
     * @param other object which info are move from
     */
    AirplaneInfo& operator=(AirplaneInfo&& other)
    {
        update_members(other);
        other.reset();
        return *this;
    }

private:

    void update_members(const AirplaneInfo& other)
    {
        _id = other._id;
        _fuel = other._fuel;
        _fuel_capacity = other._fuel_capacity;
        _fuel_consumption_rate = other._fuel_consumption_rate;

        _airport_answer_time_unit_counter = other._airport_answer_time_unit_counter;
        _airport_answer_time_unit_limit = other._airport_answer_time_unit_limit;
        _taking_off_time_unit_counter = other._taking_off_time_unit_counter;
        _taking_off_time_unit_limit = other._taking_off_time_unit_limit;
        _flying_time_unit_counter = other._flying_time_unit_counter;
        _flying_time_unit_limit = other._flying_time_unit_limit;
        _landing_time_unit_counter = other._landing_time_unit_counter;
        _landing_time_unit_limit = other._landing_time_unit_limit;
        _landed_time_unit_counter = other._landed_time_unit_counter;
        _landed_time_unit_limit = other._landed_time_unit_limit;
    }

    /**
     * Reset class members - prevent the usage of moved object (it would has the same ID)
     */
    void reset()
    {
        _id = 0;
        _fuel = 0;
        _fuel_capacity = 0;
        _fuel_consumption_rate = 0;

        _airport_answer_time_unit_counter = 0;
        _airport_answer_time_unit_limit = 0;
        _taking_off_time_unit_counter = 0;
        _taking_off_time_unit_limit = 0;
        _flying_time_unit_counter = 0;
        _flying_time_unit_limit = 0;
        _landing_time_unit_counter = 0;
        _landing_time_unit_limit = 0;
        _landed_time_unit_counter = 0;
        _landed_time_unit_limit = 0;
    }
};

/**
 * The idea is keep the class simples and using C20 from cppguideline - If you can avoid defining
 * default operations, do.
 *
 * https://en.cppreference.com/w/cpp/language/rule_of_three
 */
class Airplane : public std::enable_shared_from_this<Airplane>
{
    AirplaneInfo _info;
    AirplaneState _state;

    //  TODO: create Factory and instance Airplane from there!
    //  for now to keep it simple, using shared_ptr()
    std::shared_ptr<Airport> _airport;

    bool _is_waiting_take_off_authorization;
    bool _is_take_off_authorized;
    bool _is_waiting_landing_authorization;
    bool _is_landing_authorized;

    virtual void set_state(AirplaneState state);
    virtual bool is_fuel_consumption_state(void);

    virtual void update_state(void);

public:
    /**
     * Default Class Constructor
     */
    Airplane();

    /**
     * Class Constructor
     * @param airplane_info information about airplane provided by AirplanInfo structure
     */
    Airplane(AirplaneInfo& airplane_info);

    /**
     * A function which return the Airplane's state
     * @return Airplane state
     */
    virtual AirplaneState get_state(void);

    virtual std::uint32_t get_id(void);
    virtual std::uint32_t get_fuel(void);
    virtual std::uint32_t get_fuel_consumption_rate(void);
    virtual std::shared_ptr<Airport> get_airport(void);

    /**
     * A function which set the airport this airplane will contact
     * @param airport airport which this airplane will contact
     */
    virtual void set_airport(std::shared_ptr<Airport> airport);

    /**
     * A function which refuel airplane and set the airplane available to next trip
     */
    virtual void refuel(void);

    virtual void take_off_authorized(void);
    virtual void take_off_denied(void);

    virtual void land_authorized(void);
    virtual void land_denied(void);

    /**
     * A function which update the time unit of Airplane, this is the input function to Airplane
     * update its fuel and check if it needs to change the state. Example: if the time unit is
     * seconds, this function should be called once in 1 second.
     */
    virtual void update_time_unit(void);
};

#endif /* AIRPLANE_HPP */