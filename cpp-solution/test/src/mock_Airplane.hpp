// MockAirplane.hpp
#ifndef MOCK_AIRPLANE_HPP // include guard
#define MOCK_AIRPLANE_HPP

#include "gmock/gmock.h"  // Brings in Google Mock.

#include "Airplane.hpp"

class MockAirplane : public Airplane
{
public:
    MOCK_METHOD0(take_off_authorized, void(void));
    MOCK_METHOD0(take_off_denied, void(void));

    MOCK_METHOD0(land_authorized, void(void));
    MOCK_METHOD0(land_denied, void(void));
};

#endif /* MOCK_AIRPLANE_HPP */