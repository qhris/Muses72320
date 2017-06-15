#ifndef MOCK_SPI_HEADER
#define MOCK_SPI_HEADER

#include <queue>
#include <Arduino.h>

#define MSBFIRST 1
#define SPI_MODE2 0x08

struct SPISettings
{
	// Not actually used for testing.
	SPISettings(uint32_t, uint8_t, uint8_t) {}
};

class SPIClass
{
public:
	SPIClass() :
		initialized(false),
		isTransactionActive(false),
		writtenBytes()
	{ }

	void begin()
	{
		initialized = true;

		clearByteBuffer();

		digitalWrite(SCK, LOW);
		pinMode(SCK, OUTPUT);

		digitalWrite(MOSI, LOW);
		pinMode(MOSI, OUTPUT);

		digitalWrite(SS, HIGH);
		pinMode(SS, OUTPUT);
	}

	void beginTransaction(const SPISettings&)
	{
		isTransactionActive = true;
	}

	void endTransaction()
	{
		isTransactionActive = false;
	}

	void transfer(byte data)
	{
		if (isTransactionActive)
			writtenBytes.push(data);
	}

	bool isInitialized() const
	{
		return initialized;
	}

	std::queue<byte>& getWrittenBytes()
	{
		return writtenBytes;
	}

private:
	void clearByteBuffer()
	{
		std::queue<byte> empty;
		std::swap(writtenBytes, empty);
	}

	bool initialized;
	bool isTransactionActive;
	std::queue<byte> writtenBytes;
};

extern SPIClass SPI;

#endif // MOCK_SPI_HEADER
