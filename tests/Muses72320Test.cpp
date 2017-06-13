#include <Muses72320.h>
#include <SPI.h>

#include <catch/catch.hpp>

static byte readNextChipAddress(std::queue<byte>& queue);

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

		auto& bytes = SPI.getWrittenBytes();
		REQUIRE(readNextChipAddress(bytes) == addr);
		REQUIRE(bytes.size() == 0);
	}
}

byte readNextChipAddress(std::queue<byte>& queue)
{
	queue.pop();
	byte address = queue.front();
	queue.pop();
	return address & 0b0111;
}
