#ifndef INCLUDED_MUSES_SPI_DATA_READER
#define INCLUDED_MUSES_SPI_DATA_READER

#include <Muses72320.h>

struct MusesSPIData
{
	bool bAttenuationL;
	bool bAttenuationR;
	bool bGainL;
	bool bGainR;
	bool bState;

	Muses72320::address_t chipAddress;

	byte attenuationL = 0;
	byte attenuationR = 0;
	byte gainL = 0;
	byte gainR = 0;
	byte state = 0;
};

struct MusesSPIDataReader
{
	static byte readNext();

	static MusesSPIData readBlock() { return readBlocks(1); }
	static MusesSPIData readBlocks(size_t blocks);
};

#endif // INCLUDED_MUSES_SPI_DATA_READER
