// MockAirport.hpp
#ifndef MOCK_AIRPORT_HPP // include guard
#define MOCK_AIRPORT_HPP

#include "gmock/gmock.h"  // Brings in Google Mock.

#include "Airport.hpp"

class MockAirport : public Airport
{
public:
    MOCK_METHOD1(permission_to_take_off, void(std::shared_ptr<Airplane> airplane));
    MOCK_METHOD1(permission_to_land, void(std::shared_ptr<Airplane> airplane));
};

#endif /* MOCK_AIRPORT_HPP */