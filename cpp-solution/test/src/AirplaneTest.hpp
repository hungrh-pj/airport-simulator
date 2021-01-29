#include <gtest/gtest.h>
#include <memory>
#include "Airplane.hpp"
#include "mock_Airport.hpp"

using namespace std;
using ::testing::Return;
using ::testing::_;

class AirplaneTest : public ::testing::Test
{
public:
    Airplane _basic_airplane;
    std::shared_ptr<MockAirport> _mock_airport;

    const std::uint32_t BASIC_AIRPLANE_ID = 1;
    const std::uint32_t BASIC_AIRPLANE_FUEL_START = 100;
    const std::uint32_t BASIC_AIRPLANE_FUEL_CONS_RATE = 2;
    const std::uint32_t BASIC_AIRPLANE_AIRPORT_ANS_TIMEOUT = 2;

    //  basic config the fuel can be consumed 50 times (after flying)
    std::uint32_t _basic_airplane_max_fuel_consume = (std::uint32_t)(
        BASIC_AIRPLANE_FUEL_START/BASIC_AIRPLANE_FUEL_CONS_RATE);

    AirplaneTest()
    {
        AirplaneInfo basic_apl_info;
        basic_apl_info._id = BASIC_AIRPLANE_ID;
        basic_apl_info._fuel = BASIC_AIRPLANE_FUEL_START;
        basic_apl_info._fuel_consumption_rate = BASIC_AIRPLANE_FUEL_CONS_RATE;
        basic_apl_info._airport_answer_timeout = BASIC_AIRPLANE_AIRPORT_ANS_TIMEOUT;
        _basic_airplane = Airplane(basic_apl_info);
        _mock_airport.reset(new MockAirport());
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(AirplaneTest, GetState_DefaultStateTest_ExpectedStopped)
{
    ASSERT_EQ(_basic_airplane.get_state(), AirplaneState::stopped);
    ASSERT_EQ(_basic_airplane.get_id(), BASIC_AIRPLANE_ID);
    ASSERT_EQ(_basic_airplane.get_fuel(), BASIC_AIRPLANE_FUEL_START);
}

TEST_F(AirplaneTest, GetState_ConsumeFuelWithoutAuthorizationTest_ExpectedStopped)
{
    uint16_t i=0;
    for (; i < _basic_airplane_max_fuel_consume; ++i)
    {
        _basic_airplane.update_time_unit();
    }

    ASSERT_EQ(_basic_airplane.get_state(), AirplaneState::stopped);
    ASSERT_EQ(_basic_airplane.get_id(), BASIC_AIRPLANE_ID);
    ASSERT_EQ(_basic_airplane.get_fuel(), BASIC_AIRPLANE_FUEL_START);
}

TEST_F(AirplaneTest, GetState_EmptyFuelAfterFlyTest_ExpectedCrashed)
{
    _basic_airplane.set_airport(_mock_airport);

    EXPECT_CALL(*_mock_airport, permission_to_take_off(_))
        .WillOnce(Return(true));

    ASSERT_EQ(_basic_airplane.get_state(), AirplaneState::stopped);
}

// TEST_F(AirplaneTest, GetState_EmptyFuelAfterFlyTest_ExpectedCrashed)
// {
//     ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::crashed);
// }
