#include "gmock/gmock.h"  // Brings in Google Mock.

#include "Airport.hpp"

class MockAirport : public Airport
{
public:
    MOCK_METHOD1(permission_to_take_off, bool(Airplane& airplane));
    MOCK_METHOD1(permission_to_land, bool(Airplane& airplane));
};