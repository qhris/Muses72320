#include <utility/VolumeControlData.hpp>
#include <catch/catch.hpp>

using namespace MusesTypes;

TEST_CASE("Constructor sets data fields correctly.", "[VolumeControlData]")
{
	const VolumeControlData data(0x10, 0x20);
	REQUIRE(data.getAttenuation() == 0x10);
	REQUIRE(data.getGain() == 0x20);
}
