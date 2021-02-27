// AirplaneState.hpp
#ifndef AIRPLANE_STATE_HPP // include guard
#define AIRPLANE_STATE_HPP

enum class AirplaneState
{
    unknown = 0,
    stopped,
    asking_permission_to_take_off,
    taking_off,
    flying,
    asking_permission_to_land,
    landing,
    landed,
    crashed
};

#endif /* AIRPLANE_STATE_HPP */