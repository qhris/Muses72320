#ifndef INCLUDED_MUSES_72320_TYPES
#define INCLUDED_MUSES_72320_TYPES

#include <stdint.h>

namespace MusesTypes
{
	typedef uint8_t data_t;
	typedef int16_t volume_t;
	typedef uint8_t address_t;
}

namespace MusesDetails
{
	struct StateControlData
	{
		bool zeroCrossing;
		bool linkGain;
		bool linkAttenuation;
	};
}

#endif // INCLUDED_MUSES_72320_TYPES
