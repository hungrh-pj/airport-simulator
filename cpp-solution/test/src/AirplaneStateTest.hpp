#include <gtest/gtest.h>
#include <memory>
#include "Airplane.hpp"
#include "mock_Airport.hpp"

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::InvokeWithoutArgs;
using ::testing::AtLeast;

class AirplaneStateTest : public ::testing::Test
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

    AirplaneStateTest()
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
    }

    //  TODO: helper functions which use ASSERT_* macros must return void at google test!
    //Is it C++ streaming issue? Check the reason...
    void SetUpTakeOff()
    {
        _basic_airplane->set_airport(_mock_airport);

        //  update to advance from stopped state to asking_permission state
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);

        //  mock airport doesn't has any queue, so it just authorize airplane action in this
        //scenario
        ON_CALL(*_mock_airport, permission_to_take_off(_))
            .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::take_off_authorized));
        EXPECT_CALL(*_mock_airport, permission_to_take_off(_))
            .Times(AtLeast(1));

        //  update to take action asking for permission
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);

        //  update to advance from asking permission to taking off
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::taking_off);
    }

    void SetUpFlying()
    {
        SetUpTakeOff();

        std::uint16_t timeUnitCounter = 0;
        for (; timeUnitCounter <= BASIC_AIRPLANE_TAKING_OFF_TIME;
            ++timeUnitCounter)
        {
            _basic_airplane->update_time_unit();
            if (_basic_airplane->get_state() == AirplaneState::flying)
            {
                break;
            }
        }

        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::flying);
    }

    void FlyAroundRoutine()
    {
        std::uint16_t timeUnitCounter = 0;
        for (; timeUnitCounter < BASIC_AIRPLANE_FLYING_TIME; ++timeUnitCounter)
        {
            _basic_airplane->update_time_unit();
            ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::flying);
        }
    }

    void SetUpAskingPermissionToLand()
    {
        SetUpFlying();
        FlyAroundRoutine();

        //  update to advance from flying to asking permission to land
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_land);
    }

    void SetUpLanding()
    {
        SetUpAskingPermissionToLand();

        //  mock airport doesn't has any queue, so it just authorize airplane action in this
        //scenario
        ON_CALL(*_mock_airport, permission_to_land(_))
            .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::land_authorized));
        EXPECT_CALL(*_mock_airport, permission_to_land(_))
            .Times(AtLeast(1));

        //  update to take action asking for permission
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_land);

        //  update to advance from asking permission to land to landing
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::landing);
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(AirplaneStateTest, DefaultStateTest_ExpectedStopped)
{
    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::stopped);
}

TEST_F(AirplaneStateTest, IncreaseTimeUnitTest_ExpectedStopped)
{
    uint16_t i=0;
    for (; i < BASIC_AIRPLANE_MAX_TIME; ++i)
    {
        _basic_airplane->update_time_unit();
    }

    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);
}

TEST_F(AirplaneStateTest, NoPermissionToTakeOffTest_ExpectedAskingPermissionToTakeOff)
{
    _basic_airplane->set_airport(_mock_airport);

    //  update to advance from stopped state to asking_permission state
    _basic_airplane->update_time_unit();
    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);

    //  at least more than one attempt the airplane must ask for permission if the airport denie
    EXPECT_CALL(*_mock_airport, permission_to_take_off(_))
        .Times(AtLeast(2));

    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        //  airplane state remains asking permission
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_take_off);
    }

    //  crashed state is not expected because airplane will remain on the ground
}

TEST_F(AirplaneStateTest, PermissionToTakeOffTest_ExpectedTakingOff)
{
    SetUpTakeOff();
}

TEST_F(AirplaneStateTest, FlyingTest_ExpectedFlying)
{
    SetUpFlying();
}

TEST_F(AirplaneStateTest, PermissionToLandTest_ExpectedLanding)
{
    SetUpLanding();
}

TEST_F(AirplaneStateTest, PermissionToLandDeniedForeverTest_ExpectedCrashed)
{
    SetUpAskingPermissionToLand();

    //  mock airport doesn't has any queue, so it just authorize airplane action in this
    //scenario - AtLeast(2) because airplane has enough time to ask for permission again
    ON_CALL(*_mock_airport, permission_to_land(_))
        .WillByDefault(InvokeWithoutArgs(_basic_airplane.get(), &Airplane::land_denied));
    EXPECT_CALL(*_mock_airport, permission_to_land(_))
        .Times(AtLeast(2));

    //  update to take action asking for permission
    _basic_airplane->update_time_unit();
    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_land);

    uint16_t timeUnitCounter=0;
    for (; timeUnitCounter < BASIC_AIRPLANE_MAX_TIME; ++timeUnitCounter)
    {
        //  eventually if no permission is granted the airplane will crash
        _basic_airplane->update_time_unit();
        if (_basic_airplane->get_state() == AirplaneState::crashed)
        {
            break;
        }
        else
        {
            ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::asking_permission_to_land);
        }
    }

    //  update to crashed no authorization received and got out of fuel
    _basic_airplane->update_time_unit();
    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::crashed);
}

TEST_F(AirplaneStateTest, LandedTest_ExpectedLanded)
{
    SetUpLanding();

    std::uint16_t timeUnitCounter = 0;
    for (; timeUnitCounter < BASIC_AIRPLANE_LANDING_TIME;
        ++timeUnitCounter)
    {
        //  state remains landing
        _basic_airplane->update_time_unit();
        ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::landing);
    }

    //  update from landing to landed
    _basic_airplane->update_time_unit();
    ASSERT_EQ(_basic_airplane->get_state(), AirplaneState::landed);
}
