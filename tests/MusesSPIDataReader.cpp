#include "MusesSPIDataReader.hpp"

#include <SPI.h>
#include <stdexcept>

template<typename T> constexpr T maskChipAddress(const T& value) { return value & 0b0000'0111; }
template<typename T> constexpr T maskSelectAddress(const T& value) { return value & 0b1111'0000; }

byte MusesSPIDataReader::readNext()
{
	auto& bytes = SPI.getWrittenBytes();
	if (bytes.empty())
		throw new std::out_of_range("Invalid spi data count.");

	byte data = bytes.front();
	bytes.pop();

	return data;
}

MusesSPIData MusesSPIDataReader::readBlocks(size_t blocks)
{
	MusesSPIData spiData = {};

	for (size_t i = 0; i < blocks; ++i) {
		byte data = readNext();
		byte address = readNext();

		spiData.chipAddress = maskChipAddress(address);
		switch (maskSelectAddress(address))
		{
			case 0b0000'0000:
				spiData.bAttenuationL = true;
				spiData.attenuationL = data;
				break;

			case 0b0001'0000:
				spiData.bGainL = true;
				spiData.gainL = data;
				break;

			case 0b0010'0000:
				spiData.bAttenuationR = true;
				spiData.attenuationR = data;
				break;

			case 0b0011'0000:
				spiData.bGainR = true;
				spiData.gainR = data;
				break;

			case 0b0100'0000:
				spiData.bState = true;
				spiData.state = data;
				break;

			default: break;
		}
	}

	return spiData;
}
