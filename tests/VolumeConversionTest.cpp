#include <VolumeConversion.hpp>
#include <catch/catch.hpp>

TEST_CASE("Volume is converted to the correct attenuation bits.", "[VolumeConversion]")
{
	REQUIRE(volume_to_attenuation(0) == (data_t)0x10);
	REQUIRE(volume_to_attenuation(-1) == (data_t)0x11);
	REQUIRE(volume_to_attenuation(-2) == (data_t)0x12);
	REQUIRE(volume_to_attenuation(-3) == (data_t)0x13);
	REQUIRE(volume_to_attenuation(-100) == (data_t)0x74);
}

TEST_CASE("Converting volume to attenuation clamps volume to the correct range", "[VolumeConversion]")
{
	REQUIRE(volume_to_attenuation(0) == (data_t)0b10000); // 0dB
	REQUIRE(volume_to_attenuation(1) == (data_t)0b10000);
	REQUIRE(volume_to_attenuation(100) == (data_t)0b10000);
	REQUIRE(volume_to_attenuation(-223) == (data_t)0b11101111); // -111.5dB
	REQUIRE(volume_to_attenuation(-224) == (data_t)0b11101111);
	REQUIRE(volume_to_attenuation(-300) == (data_t)0b11101111);
}
