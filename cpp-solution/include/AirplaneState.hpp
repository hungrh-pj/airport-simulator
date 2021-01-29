// AirplaneState.hpp
#ifndef AIRPLANE_STATE_HPP // include guard
#define AIRPLANE_STATE_HPP

enum class AirplaneState
{
    stopped = 0,
    asking_permission_to_take_off,
    taking_off,
    flying,
    asking_permission_to_land,
    landing,
    crashed
};

#endif /* AIRPLANE_STATE_HPP */