#include <utility/AudioControlData.hpp>
#include <utility/AudioControlDataConverter.hpp>
#include <catch/catch.hpp>

static const byte ATTENUATION_DATA_ZERO = 0b10000;
static const byte GAIN_DATA_ZERO = 0b0;
// The `fine gain` control data allows a special +0.25dB instead of +0.5dB.
static const byte GAIN_DATA_FINE = 0b1000000;

// Helper macros to make tests easier to read.
#define convertAttenuationToAttenuationData(attenuation)\
	AudioControlDataConverter::fromAttenuation((attenuation)).getAttenuationData()
#define convertAttenuationToGainData(attenuation)\
	AudioControlDataConverter::fromAttenuation((attenuation)).getGainData()
#define convertGainToGainData(gain)\
	AudioControlDataConverter::fromGain((gain)).getGainData()
#define convertGainToAttenuationData(gain)\
	AudioControlDataConverter::fromGain((gain)).getAttenuationData()
#define convertVolumeToAttenuationData(volume)\
	AudioControlDataConverter::fromVolume((volume)).getAttenuationData()
#define convertVolumeToGainData(volume)\
	AudioControlDataConverter::fromVolume((volume)).getGainData()

TEST_CASE("Creates correct attenuation control data for attenuation values.", "[AudioControlDataConverter]")
{
	REQUIRE(convertAttenuationToAttenuationData(0) == 0x10);
	REQUIRE(convertAttenuationToAttenuationData(-1) == 0x11);
	REQUIRE(convertAttenuationToAttenuationData(-2) == 0x12);
	REQUIRE(convertAttenuationToAttenuationData(-3) == 0x13);
	REQUIRE(convertAttenuationToAttenuationData(-100) == 0x74);
}

TEST_CASE("Clamps attenuation to the correct range", "[AudioControlDataConverter]")
{
	REQUIRE(convertAttenuationToAttenuationData(0) == ATTENUATION_DATA_ZERO); // 0dB
	REQUIRE(convertAttenuationToAttenuationData(1) == ATTENUATION_DATA_ZERO);
	REQUIRE(convertAttenuationToAttenuationData(100) == ATTENUATION_DATA_ZERO);
	REQUIRE(convertAttenuationToAttenuationData(-223) == 0b11101111); // -111.5dB
	REQUIRE(convertAttenuationToAttenuationData(-224) == 0b11101111);
	REQUIRE(convertAttenuationToAttenuationData(-300) == 0b11101111);
}

TEST_CASE("Creates control data with zeroed gain for attenuation.", "[AudioControlDataConverter]")
{
	REQUIRE(convertAttenuationToGainData(-1) == GAIN_DATA_ZERO);
	REQUIRE(convertAttenuationToGainData(-2) == GAIN_DATA_ZERO);
	REQUIRE(convertAttenuationToGainData(-300) == GAIN_DATA_ZERO);
}

TEST_CASE("Creates correct gain control data for gain values.", "[AudioControlDataConverter]")
{
	REQUIRE(convertGainToGainData(0) == 0);
	REQUIRE(convertGainToGainData(1) == 1);
	REQUIRE(convertGainToGainData(63) == 0b0011'1111);
}

TEST_CASE("Clamps gain values within the correct range.", "[AudioControlDataConverter]")
{
	REQUIRE(convertGainToGainData(-1) == 0b0);
	REQUIRE(convertGainToGainData(-100) == 0b0);
	REQUIRE(convertGainToGainData(64) == 0b111111);
	REQUIRE(convertGainToGainData(100) == 0b111111);
}

TEST_CASE("Creates control data with zero attenuation for gain.", "[AudioControlDataConverter]")
{
	REQUIRE(convertGainToAttenuationData(1) == ATTENUATION_DATA_ZERO);
	REQUIRE(convertGainToAttenuationData(63) == ATTENUATION_DATA_ZERO);
}

TEST_CASE("Creates control data with corrects values for fine gain.", "[AudioControlDataConverter]")
{
	const auto controlData = AudioControlDataConverter::fromFineGain();
	REQUIRE(controlData.getAttenuationData() == ATTENUATION_DATA_ZERO);
	REQUIRE(controlData.getGainData() == GAIN_DATA_FINE);
}

TEST_CASE("Creates correct attenuation and gain values for volume.", "[AudioControlDataConverter]")
{
	REQUIRE(convertVolumeToAttenuationData(0) == ATTENUATION_DATA_ZERO);
	REQUIRE(convertVolumeToGainData(0) == GAIN_DATA_ZERO);

	REQUIRE(convertVolumeToAttenuationData(-1) == 0b10001);
	REQUIRE(convertVolumeToGainData(-1) == GAIN_DATA_FINE);

	REQUIRE(convertVolumeToAttenuationData(-2) == 0b10001);
	REQUIRE(convertVolumeToGainData(-2) == GAIN_DATA_ZERO);

	REQUIRE(convertVolumeToAttenuationData(-3) == 0b10010);
	REQUIRE(convertVolumeToGainData(-3) == GAIN_DATA_FINE);

	REQUIRE(convertVolumeToAttenuationData(-445) == 0b11101111);
	REQUIRE(convertVolumeToGainData(-445) == GAIN_DATA_FINE);

	REQUIRE(convertVolumeToAttenuationData(-446) == 0b11101111);
	REQUIRE(convertVolumeToGainData(-446) == GAIN_DATA_ZERO);
}

TEST_CASE("Clamps volume within the correct range.", "[AudioControlDataConverter]")
{
	REQUIRE(convertVolumeToAttenuationData(1) == ATTENUATION_DATA_ZERO);
	REQUIRE(convertVolumeToGainData(1) == GAIN_DATA_ZERO);

	REQUIRE(convertVolumeToAttenuationData(-447) == 0b11101111);
	REQUIRE(convertVolumeToGainData(-447) == GAIN_DATA_ZERO);
}
