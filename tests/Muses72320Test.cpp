#include <Muses72320.h>
#include <SPI.h>

#include <catch/catch.hpp>
#include "MusesSPIDataReader.hpp"

static Muses72320 createInitializedDevice(byte address = 0)
{
	Muses72320 device(address);
	device.begin();

	return device;
}

TEST_CASE("Correctly sets the default values.", "[Muses72320]")
{
	Muses72320 device(0);

	REQUIRE(device.isZeroCrossingEnabled() == true);
	REQUIRE(device.isGainLinked() == false);
	REQUIRE(device.isAttenuationLinked() == false);
}

TEST_CASE("Initializing Muses also initializes the SPI library.", "[Muses72320]")
{
	Muses72320 device(0);

	REQUIRE(SPI.isInitialized() == false);

	device.begin();
	REQUIRE(SPI.isInitialized() == true);
	REQUIRE(mockReadPinMode(SCK) == OUTPUT);
	REQUIRE(mockReadPinMode(MOSI) == OUTPUT);
	REQUIRE(mockReadPinMode(SS) == OUTPUT);
	REQUIRE(digitalRead(SS) == HIGH);
}

TEST_CASE("SPI tranfsers to the correct muses address.", "[Muses72320]")
{
	for (byte addr = 0; addr <= 0b111; addr++)
	{
		auto device = createInitializedDevice(addr);

		device.setAttenuationLeft(0);
		auto data = MusesSPIDataReader::readBlock();
		REQUIRE(data.chipAddress == addr);
	}
}

TEST_CASE("Attenuation is sent on the correct select address.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setAttenuationLeft(0);
	REQUIRE(MusesSPIDataReader::readBlock().bAttenuationL);

	device.setAttenuationRight(0);
	REQUIRE(MusesSPIDataReader::readBlock().bAttenuationR);
}

TEST_CASE("Gain is sent on the correct select address.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setGainLeft(0);
	REQUIRE(MusesSPIDataReader::readBlock().bGainL);

	device.setGainRight(0);
	REQUIRE(MusesSPIDataReader::readBlock().bGainR);
}

TEST_CASE("Volume is sent with both attenuation and gain control.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setVolume(0);
	auto data = MusesSPIDataReader::readBlocks(4);

	REQUIRE(data.bAttenuationL);
	REQUIRE(data.bAttenuationR);
	REQUIRE(data.bGainL);
	REQUIRE(data.bGainR);
}

TEST_CASE("State is sent with the correct select address.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.enableAttenuationLink();
	REQUIRE(MusesSPIDataReader::readBlock().bState);
}

TEST_CASE("Mute data is sent correctly.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.mute();
	auto data = MusesSPIDataReader::readBlocks(2);
	REQUIRE(data.bAttenuationL == true);
	REQUIRE(data.bAttenuationR == true);
	REQUIRE(data.attenuationL == 0);
	REQUIRE(data.attenuationR == 0);
}

TEST_CASE("Attenuation values are sent.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setAttenuation(-7);
	auto data = MusesSPIDataReader::readBlocks(2);
	REQUIRE(data.attenuationL == 0x17);
	REQUIRE(data.attenuationR == 0x17);
}

TEST_CASE("Gain values are sent.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setGain(7);
	auto data = MusesSPIDataReader::readBlocks(2);
	REQUIRE(data.gainL == 0x7);
	REQUIRE(data.gainR == 0x7);
}

TEST_CASE("Volume controls 0.25dB steps.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setVolume(-3);

	auto data = MusesSPIDataReader::readBlocks(4);
	REQUIRE(data.attenuationL == 0b0001'0010);
	REQUIRE(data.attenuationR == 0b0001'0010);
	REQUIRE(data.gainL == 0b0100'0000);
	REQUIRE(data.gainR == 0b0100'0000);
}

TEST_CASE("Can set loweset attenuation", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setAttenuation(-223);
	auto data = MusesSPIDataReader::readBlocks(2);
	REQUIRE(data.attenuationL == 0b1110'1111);
	REQUIRE(data.attenuationR == 0b1110'1111);
}

TEST_CASE("Can set highest gain.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setGain(63);
	auto data = MusesSPIDataReader::readBlocks(2);
	REQUIRE(data.gainL == 0b0011'1111);
	REQUIRE(data.gainR == 0b0011'1111);
}

TEST_CASE("Can set lowest volume.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.setVolume(-446);
	auto data = MusesSPIDataReader::readBlocks(4);
	REQUIRE(data.attenuationL == 0b1110'1111);
	REQUIRE(data.attenuationR == 0b1110'1111);
	REQUIRE(data.gainL == 0);
	REQUIRE(data.gainR == 0);
}

TEST_CASE("Zero crossing data is valid.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.disableZeroCrossing();
	REQUIRE_FALSE(device.isZeroCrossingEnabled());
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b0010'0000);
	device.enableZeroCrossing();
	REQUIRE(device.isZeroCrossingEnabled());
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b0000'0000);
}

TEST_CASE("Attenuation link data is valid.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.enableAttenuationLink();
	REQUIRE(device.isAttenuationLinked());
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b1000'0000);
	device.disableAttenuationLink();
	REQUIRE_FALSE(device.isAttenuationLinked());
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b0000'0000);
}

TEST_CASE("Gain link data is valid.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.enableGainLink();
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b0100'0000);
	REQUIRE(device.isGainLinked());
	device.disableGainLink();
	REQUIRE(MusesSPIDataReader::readBlock().state == 0b0000'0000);
	REQUIRE_FALSE(device.isGainLinked());
}

TEST_CASE("State is correctly combined.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.disableZeroCrossing();
	device.enableGainLink();
	device.enableAttenuationLink();

	REQUIRE(MusesSPIDataReader::readBlocks(3).state == 0b1110'0000);
}

TEST_CASE("Right channel is ignored when attenuation is linked.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.enableAttenuationLink();
	REQUIRE_NOTHROW(MusesSPIDataReader::readBlock());
	device.setAttenuationRight(0);
	REQUIRE_THROWS(MusesSPIDataReader::readBlock());
}

TEST_CASE("Right channel is ignored when gain is linked.", "[Muses72320]")
{
	auto device = createInitializedDevice();

	device.enableGainLink();
	REQUIRE_NOTHROW(MusesSPIDataReader::readBlock());
	device.setGainRight(0);
	REQUIRE_THROWS(MusesSPIDataReader::readBlock());
}
