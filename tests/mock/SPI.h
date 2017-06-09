#ifndef MOCK_SPI_HEADER
#define MOCK_SPI_HEADER

#include <Arduino.h>

#define MSBFIRST 1
#define SPI_MODE2 0x08

struct SPISettings
{
	// Not actually used for testing.
	SPISettings(uint32_t, uint8_t, uint8_t) {}
};

struct SPIClass
{
	void begin() {}
	void beginTransaction(const SPISettings&) {}
	void endTransaction() {}
	void transfer(byte data) {}
};

extern SPIClass SPI;

#endif // MOCK_SPI_HEADER
