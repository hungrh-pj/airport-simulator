#include <gtest/gtest.h>

#include "AirplaneStateTest.hpp"
#include "AirplaneFuelConsumptionTest.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
