#include <Muses72320.h>
#include <SPI.h>

#include <catch/catch.hpp>

static void clearSPIBytes();
static byte readSPINextByte();
static byte readSPINextSelectAddressWithSkip();
static byte readSPINextSelectAddress();
static byte readSPINextChipAddress();
static size_t getSPIByteCount();

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
	for (Muses72320::address_t addr = 0; addr <= 0b111; addr++)
	{
		Muses72320 device(addr);
		device.begin();
		device.setAttenuationLeft(0);

		REQUIRE(getSPIByteCount() == 2);
		REQUIRE(readSPINextChipAddress() == addr);
	}
}

TEST_CASE("Attenuation is sent on the correct select address.", "[Muses72320]")
{
	Muses72320 device(0);
	device.begin();

	device.setAttenuationLeft(0);
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextSelectAddressWithSkip() == 0);

	device.setAttenuationRight(0);
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextSelectAddressWithSkip() == 0b0010'0000);
}

TEST_CASE("Gain is sent on the correct select address.", "[Muses72320]")
{
	Muses72320 device(0);
	device.begin();

	device.setGainLeft(0);
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextSelectAddressWithSkip() == 0b0001'0000);

	device.setGainRight(0);
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextSelectAddressWithSkip() == 0b0011'0000);
}

TEST_CASE("Volume is sent with both attenuation and gain control.", "[Muses72320]")
{
	Muses72320 device(0);
	device.begin();

	device.setVolumeLeft(0);
	device.setVolumeRight(0);

	bool bAttenuationL = false;
	bool bAttenuationR = false;
	bool bGainL = false;
	bool bGainR = false;

	REQUIRE(getSPIByteCount() == 8);

	for (size_t i = 0; i < 4; ++i) {
		byte selectAddress = readSPINextSelectAddressWithSkip();

		switch (selectAddress)
		{
			case 0b0000'0000: bAttenuationL = true; break;
			case 0b0001'0000: bGainL = true; break;
			case 0b0010'0000: bAttenuationR = true; break;
			case 0b0011'0000: bGainR = true; break;
			default: break;
		}
	}

	REQUIRE(bAttenuationL);
	REQUIRE(bAttenuationR);
	REQUIRE(bGainL);
	REQUIRE(bGainR);
}

TEST_CASE("State is sent with the correct select address.", "[Muses72320]")
{
	Muses72320 device(0);
	device.begin();

	device.enableAttenuationLink();
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextSelectAddressWithSkip() == 0b0100'0000);
}

TEST_CASE("Mute data is sent correctly.", "[Muses72320]")
{
	Muses72320 device(0);
	device.begin();

	device.muteLeft();
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextByte() == 0);
	REQUIRE(readSPINextSelectAddress() == 0b0000'0000);

	device.muteRight();
	REQUIRE(getSPIByteCount() == 2);
	REQUIRE(readSPINextByte() == 0);
	REQUIRE(readSPINextSelectAddress() == 0b0010'0000);
}

void clearSPIBytes()
{
	auto& bytes = SPI.getWrittenBytes();
	while (!bytes.empty()) bytes.pop();
}

byte readSPINextByte()
{
	auto& queue = SPI.getWrittenBytes();
	byte value = queue.front();
	queue.pop();
	return value;
}

byte readSPINextSelectAddressWithSkip()
{
	readSPINextByte();
	return readSPINextSelectAddress();
}

byte readSPINextSelectAddress()
{
	return readSPINextByte() & 0b11110000;
}

byte readSPINextChipAddress()
{
	readSPINextByte();
	return readSPINextByte() & 0b0111;
}

size_t getSPIByteCount()
{
	return SPI.getWrittenBytes().size();
}
