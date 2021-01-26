#include <gtest/gtest.h>

#include "Airplane.hpp"

TEST (AirplaneTest, GetState_DefaultTest_ExpectedStopped) {
    Airplane airplane(1,2,3,4);
    AirplaneState state = airplane.get_state();
    ASSERT_EQ(state, AIRPLANE_STOPPED);
}
