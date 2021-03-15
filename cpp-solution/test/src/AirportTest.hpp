#include <gtest/gtest.h>
#include <memory>
#include "Airport.hpp"
#include "mock_Airplane.hpp"

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::InvokeWithoutArgs;
using ::testing::AtLeast;

class AirportTest : public ::testing::Test
{
public:

    AirportTest()
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(AirportTest, SimpleAuthorizationToTakeOff_ExpectAirplaneHasPermissionToTakeOff)
{
    std::uint16_t numberRunWays = 1;
    std::shared_ptr<Airport> _basic_airport = std::make_shared<Airport>(numberRunWays);
    std::shared_ptr<MockAirplane> airplane1 = std::make_shared<MockAirplane>();

    //  it is expected to received the authorization because the airport has 1 runway and it has
    //no one using the runway
    EXPECT_CALL(*airplane1, take_off_authorized())
        .Times(AtLeast(1));

    _basic_airport->permission_to_take_off(airplane1);
}

TEST_F(AirportTest, SimpleAuthorizationToLand_ExpectAirplaneGetPermissionToLan)
{
    std::uint16_t numberRunWays = 1;
    std::shared_ptr<Airport> _basic_airport = std::make_shared<Airport>(numberRunWays);
    std::shared_ptr<MockAirplane> airplane1 = std::make_shared<MockAirplane>();

    //  it is expected to received the authorization because the airport has 1 runway and it has
    //no one using the runway
    EXPECT_CALL(*airplane1, land_authorized())
        .Times(AtLeast(1));

    _basic_airport->permission_to_land(airplane1);
}
