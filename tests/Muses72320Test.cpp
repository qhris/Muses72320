#include <Muses72320.h>
#include <catch/catch.hpp>

TEST_CASE("Correctly sets the default values.", "[Muses72320]")
{
	Muses72320 device(0);

	REQUIRE(device.isZeroCrossingEnabled() == true);
	REQUIRE(device.isGainLinked() == false);
	REQUIRE(device.isAttenuationLinked() == false);
}
