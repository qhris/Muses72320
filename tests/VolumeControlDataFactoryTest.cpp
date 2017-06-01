#include <VolumeControlDataFactory.hpp>
#include <catch/catch.hpp>

TEST_CASE("Factory creates the right control data for attenuation values.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromAttenuation(0).getAttenuation() == (data_t)0x10);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-1).getAttenuation() == (data_t)0x11);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-2).getAttenuation() == (data_t)0x12);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-3).getAttenuation() == (data_t)0x13);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-100).getAttenuation() == (data_t)0x74);
}

TEST_CASE("Factory clamps attenuation to the correct range", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromAttenuation(0).getAttenuation() == (data_t)0b10000); // 0dB
	REQUIRE(VolumeControlDataFactory::fromAttenuation(1).getAttenuation() == (data_t)0b10000);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(100).getAttenuation() == (data_t)0b10000);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-223).getAttenuation() == (data_t)0b11101111); // -111.5dB
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-224).getAttenuation() == (data_t)0b11101111);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-300).getAttenuation() == (data_t)0b11101111);
}
