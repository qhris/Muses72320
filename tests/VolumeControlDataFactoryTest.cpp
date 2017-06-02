#include <VolumeControlData.hpp>
#include <VolumeControlDataFactory.hpp>
#include <catch/catch.hpp>

using namespace MusesTypes;

static const data_t ATTENUATION_DATA_ZERO = 0b10000;
static const data_t GAIN_DATA_ZERO = 0b0;
// The `fine gain` control data allows a special +0.25dB instead of +0.5dB.
static const data_t GAIN_DATA_FINE = 0b1000000;

TEST_CASE("Factory creates correct attenuation control data for attenuation values.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromAttenuation(0).getAttenuation() == (data_t)0x10);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-1).getAttenuation() == (data_t)0x11);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-2).getAttenuation() == (data_t)0x12);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-3).getAttenuation() == (data_t)0x13);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-100).getAttenuation() == (data_t)0x74);
}

TEST_CASE("Factory clamps attenuation to the correct range", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromAttenuation(0).getAttenuation() == ATTENUATION_DATA_ZERO); // 0dB
	REQUIRE(VolumeControlDataFactory::fromAttenuation(1).getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(100).getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-223).getAttenuation() == (data_t)0b11101111); // -111.5dB
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-224).getAttenuation() == (data_t)0b11101111);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-300).getAttenuation() == (data_t)0b11101111);
}

TEST_CASE("Factory creates control data with zeroed gain for attenuation.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-1).getGain() == GAIN_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-2).getGain() == GAIN_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromAttenuation(-300).getGain() == GAIN_DATA_ZERO);
}

TEST_CASE("Factory creates correct gain control data for gain values.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromGain(0).getGain() == (data_t)0b0);
	REQUIRE(VolumeControlDataFactory::fromGain(1).getGain() == (data_t)0b1);
	REQUIRE(VolumeControlDataFactory::fromGain(63).getGain() == (data_t)0b111111);
}

TEST_CASE("Factory clamps gain values within the correct range.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromGain(-1).getGain() == (data_t)0b0);
	REQUIRE(VolumeControlDataFactory::fromGain(-100).getGain() == (data_t)0b0);
	REQUIRE(VolumeControlDataFactory::fromGain(64).getGain() == (data_t)0b111111);
	REQUIRE(VolumeControlDataFactory::fromGain(100).getGain() == (data_t)0b111111);
}

TEST_CASE("Factory creates control data with zero attenuation for gain.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromGain(1).getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromGain(63).getAttenuation() == ATTENUATION_DATA_ZERO);
}

TEST_CASE("Factory creates control data with corrects values for fine gain.", "[VolumeControlDataFactory]")
{
	const auto controlData = VolumeControlDataFactory::fromFineGain();
	REQUIRE(controlData.getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(controlData.getGain() == GAIN_DATA_FINE);
}

TEST_CASE("Factory creates correct attenuation and gain values for volume.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromVolume(0).getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromVolume(0).getGain() == GAIN_DATA_ZERO);

	REQUIRE(VolumeControlDataFactory::fromVolume(-1).getAttenuation() == 0b10001);
	REQUIRE(VolumeControlDataFactory::fromVolume(-1).getGain() == GAIN_DATA_FINE);

	REQUIRE(VolumeControlDataFactory::fromVolume(-2).getAttenuation() == 0b10001);
	REQUIRE(VolumeControlDataFactory::fromVolume(-2).getGain() == GAIN_DATA_ZERO);

	REQUIRE(VolumeControlDataFactory::fromVolume(-3).getAttenuation() == 0b10010);
	REQUIRE(VolumeControlDataFactory::fromVolume(-3).getGain() == GAIN_DATA_FINE);

	REQUIRE(VolumeControlDataFactory::fromVolume(-445).getAttenuation() == 0b11101111);
	REQUIRE(VolumeControlDataFactory::fromVolume(-445).getGain() == GAIN_DATA_FINE);

	REQUIRE(VolumeControlDataFactory::fromVolume(-446).getAttenuation() == 0b11101111);
	REQUIRE(VolumeControlDataFactory::fromVolume(-446).getGain() == GAIN_DATA_ZERO);
}

TEST_CASE("Factory clamps volume within the correct range.", "[VolumeControlDataFactory]")
{
	REQUIRE(VolumeControlDataFactory::fromVolume(1).getAttenuation() == ATTENUATION_DATA_ZERO);
	REQUIRE(VolumeControlDataFactory::fromVolume(1).getGain() == GAIN_DATA_ZERO);

	REQUIRE(VolumeControlDataFactory::fromVolume(-447).getAttenuation() == 0b11101111);
	REQUIRE(VolumeControlDataFactory::fromVolume(-447).getGain() == GAIN_DATA_ZERO);
}
