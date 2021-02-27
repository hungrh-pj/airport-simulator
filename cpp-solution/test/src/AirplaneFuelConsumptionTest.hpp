#include <gtest/gtest.h>
#include <memory>
#include "Airplane.hpp"
#include "mock_Airport.hpp"

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::InvokeWithoutArgs;
using ::testing::AtLeast;

class AirplaneFuelConsumptionTest : public ::testing::Test
{
public:
    std::shared_ptr<Airplane> _basic_airplane;
    std::shared_ptr<MockAirport> _mock_airport;

    const std::uint32_t BASIC_AIRPLANE_ID = 1;
    const std::uint32_t BASIC_AIRPLANE_FUEL_START = 100;
    const std::uint32_t BASIC_AIRPLANE_FUEL_CONS_RATE = 2;
    const std::uint32_t BASIC_AIRPLANE_AIRPORT_ANS_TIMEOUT = 5;
    const std::uint32_t BASIC_AIRPLANE_TAKING_OFF_TIME = 5;
    const std::uint32_t BASIC_AIRPLANE_FLYING_TIME = 30;
    const std::uint32_t BASIC_AIRPLANE_LANDING_TIME = 5;
    const std::uint32_t BASIC_AIRPLANE_LANDED_TIME = 20;
    const std::uint32_t BASIC_AIRPLANE_MAX_TIME = (uint32_t)(
        BASIC_AIRPLANE_FUEL_START/BASIC_AIRPLANE_FUEL_CONS_RATE);

    std::uint32_t _basic_airplane_time_unit_counter;

    AirplaneFuelConsumptionTest()
    {
        AirplaneInfo basic_apl_info;
        basic_apl_info._id = BASIC_AIRPLANE_ID;
        basic_apl_info._fuel = BASIC_AIRPLANE_FUEL_START;
        basic_apl_info._fuel_consumption_rate = BASIC_AIRPLANE_FUEL_CONS_RATE;
        basic_apl_info._airport_answer_time_unit_limit = BASIC_AIRPLANE_AIRPORT_ANS_TIMEOUT;
        basic_apl_info._taking_off_time_unit_limit = BASIC_AIRPLANE_TAKING_OFF_TIME;
        basic_apl_info._flying_time_unit_limit = BASIC_AIRPLANE_FLYING_TIME;
        basic_apl_info._landing_time_unit_limit = BASIC_AIRPLANE_LANDING_TIME;
        basic_apl_info._landed_time_unit_limit = BASIC_AIRPLANE_LANDED_TIME;
        _basic_airplane = std::make_shared<Airplane>(basic_apl_info);
        _mock_airport = std::make_shared<MockAirport>();

        _basic_airplane_time_unit_counter = 0;
    }

    void SetUpTakeOff()
    {
        _basic_airplane->set_airport(_mock_airport);
        _basic_airplane->update_time_unit();
        ++_basic_airplane_time_unit_counter;

        ON_CALL(*_mock_airport, permission_to_take_off(_))
            .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::take_off_authorized));
        EXPECT_CALL(*_mock_airport, permission_to_take_off(_))
            .Times(AtLeast(1));

        std::uint16_t timeUnitCounter = 0;
        bool shouldStop = false;
        for (; timeUnitCounter <= BASIC_AIRPLANE_TAKING_OFF_TIME;
            ++timeUnitCounter)
        {
            if (_basic_airplane->get_state() == AirplaneState::taking_off)
            {
                shouldStop = true;
            }

            //  after airplane goes to taking off it needs one more increment of time unit to start
            //fuel consumption
            _basic_airplane->update_time_unit();
            ++_basic_airplane_time_unit_counter;

            if (shouldStop)
            {
                break;
            }
        }

        ASSERT_NE(_basic_airplane->get_fuel(), BASIC_AIRPLANE_FUEL_START);
    }

    void SetUpFlying()
    {
        SetUpTakeOff();

        uint32_t fuelAfterTakeOff = _basic_airplane->get_fuel();
        std::uint16_t timeUnitCounter = 0;
        for (; timeUnitCounter <= BASIC_AIRPLANE_TAKING_OFF_TIME;
            ++timeUnitCounter)
        {
            _basic_airplane->update_time_unit();
            ++_basic_airplane_time_unit_counter;
            if (_basic_airplane->get_state() == AirplaneState::flying)
            {
                break;
            }
        }

        ASSERT_NE(_basic_airplane->get_fuel(), fuelAfterTakeOff);
    }

    void AskToLandFuelAt50Porcent()
    {
        uint32_t beginFuel = _basic_airplane->get_fuel();
        uint16_t fuelTolerance = BASIC_AIRPLANE_MAX_TIME / 2;
        ASSERT_LT(_basic_airplane_time_unit_counter, fuelTolerance);

        for (; _basic_airplane_time_unit_counter < fuelTolerance; ++_basic_airplane_time_unit_counter)
        {
            _basic_airplane->update_time_unit();
        }

        ASSERT_NE(_basic_airplane->get_fuel(), beginFuel);
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(AirplaneFuelConsumptionTest, JustIncreaseTimeUnit_ExpectedNoFuelConsumption)
{
    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        _basic_airplane->update_time_unit();
    }

    ASSERT_EQ(_basic_airplane->get_fuel(), BASIC_AIRPLANE_FUEL_START);
}

TEST_F(AirplaneFuelConsumptionTest, NoPermissionToTakeOffTest_ExpectedNoFuelConsumption)
{
    _basic_airplane->set_airport(_mock_airport);

    EXPECT_CALL(*_mock_airport, permission_to_take_off(_))
        .Times(AtLeast(1));

    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        _basic_airplane->update_time_unit();
    }

    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);
    ASSERT_EQ(_basic_airplane->get_fuel(), BASIC_AIRPLANE_FUEL_START);
}

TEST_F(AirplaneFuelConsumptionTest, PermissionToTakeOffTest_ExpectedFuelConsumption)
{
    SetUpTakeOff();
}

TEST_F(AirplaneFuelConsumptionTest, FlyingTest_ExpectedFuelConsumption)
{
    SetUpFlying();
}

TEST_F(AirplaneFuelConsumptionTest, PermissionToLandTest_ExpectedFuelConsumption)
{
    SetUpFlying();
    AskToLandFuelAt50Porcent();

    ON_CALL(*_mock_airport, permission_to_land(_))
        .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::land_authorized));
    EXPECT_CALL(*_mock_airport, permission_to_land(_))
        .Times(AtLeast(1));

    AirplaneState previousState = _basic_airplane->get_state();
    uint32_t fuelPreviousState = _basic_airplane->get_fuel();
    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        if (_basic_airplane->get_state() != previousState)
        {
            ASSERT_NE(_basic_airplane->get_fuel(), fuelPreviousState);
            fuelPreviousState = _basic_airplane->get_fuel();
            previousState = _basic_airplane->get_state();
        }

        _basic_airplane->update_time_unit();

        if (_basic_airplane->get_state() == AirplaneState::landed)
        {
            break;
        }
    }
}

TEST_F(AirplaneFuelConsumptionTest, NoPermissionToLandTest_ExpectedEmptyFuel)
{
    SetUpFlying();
    AskToLandFuelAt50Porcent();

    ON_CALL(*_mock_airport, permission_to_land(_))
        .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::land_denied));
    EXPECT_CALL(*_mock_airport, permission_to_land(_))
        .Times(AtLeast(1));

    AirplaneState previousState = _basic_airplane->get_state();
    uint32_t fuelPreviousState = _basic_airplane->get_fuel();
    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        if (_basic_airplane->get_state() != previousState)
        {
            ASSERT_NE(_basic_airplane->get_fuel(), fuelPreviousState);
            fuelPreviousState = _basic_airplane->get_fuel();
            previousState = _basic_airplane->get_state();
        }

        _basic_airplane->update_time_unit();

        if (_basic_airplane->get_state() == AirplaneState::crashed)
        {
            break;
        }
    }

    ASSERT_EQ(_basic_airplane->get_fuel(), (uint32_t)0);
}
