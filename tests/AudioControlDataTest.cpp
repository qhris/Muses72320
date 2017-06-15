#include <utility/AudioControlData.hpp>
#include <catch/catch.hpp>

TEST_CASE("Constructor sets data fields correctly.", "[AudioControlData]")
{
	const AudioControlData data(0x10, 0x20);
	REQUIRE(data.getAttenuationData() == 0x10);
	REQUIRE(data.getGainData() == 0x20);
}
