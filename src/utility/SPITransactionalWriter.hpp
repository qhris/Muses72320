#ifndef INCLUDED_SPI_TRANSACTIONAL_WRITER
#define INCLUDED_SPI_TRANSACTIONAL_WRITER

#include <Arduino.h>
#include <SPI.h>

class SPITransactionalWriter
{
public:
	SPITransactionalWriter(const SPISettings& settings, byte slaveSelectPin) :
		slaveSelectPin(slaveSelectPin)
	{
		SPI.beginTransaction(settings);
	}

	~SPITransactionalWriter()
	{
		SPI.endTransaction();
	}

	void write(byte chipAddress, byte selectAddress, byte data)
	{
		const byte address = chipAddress | selectAddress;

		digitalWrite(slaveSelectPin, LOW);
		SPI.transfer(data);
		SPI.transfer(address);
		digitalWrite(slaveSelectPin, HIGH);
	}

private:
	byte slaveSelectPin;
};

#endif
