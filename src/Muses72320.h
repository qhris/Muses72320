/*
The MIT License (MIT)

Copyright (c) 2016 Christoffer Hjalmarsson

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef INCLUDED_MUSES_72320
#define INCLUDED_MUSES_72320

#include <Arduino.h>
#include "MusesTypes.h"

namespace MusesDetails
{
	struct StateControlData
	{
		bool zeroCrossing;
		bool linkGain;
		bool linkAttenuation;
	};
}

class Muses72320
{
public:
	using volume_t = MusesTypes::volume_t;
	using address_t = MusesTypes::address_t;

	Muses72320(address_t chipAddress);

	/**
	 * Initializes the pin modes to enable communication with the chip.
	 * Uses digital pin 10 as the slave select pin and enables SPI.
	 */
	void begin();

	/**
	 * Simple volume control with 0.25dB steps utilizing the 0.25dB gain.
	 *
	 * Volume control range goes from -111.5dB to 0dB using this function.
	 * Output dB is calculated as: 0.25dB * volume.
	 * Input values goes from -446 to 0.
	 *
	 * Note that the 0.25dB gain step is used to achieve the precision.
	 *
	 * Example:
	 * Input of    0: -0.25 *   0 =    0.00dB [Max volume without gain]
	 * Input of -105: -0.25 * 105 =  -26.25dB
	 * Input of -446: -0.25 * 446 = -111.50dB [Minimum volume]
	 */
	void setVolume(volume_t volume)
	{
		setVolumeLeft(volume);
		setVolumeRight(volume);
	}

	void setVolumeLeft(volume_t volume);
	void setVolumeRight(volume_t volume);

	/**
	 * Direct attenuation control of the chip only allowing 0.5dB steps.
	 *
	 * Attenuation control ranges from -111.5 to 0dB using this function.
	 * Output dB is calculated as: 0.5dB * attenuation.
	 * Input values goes from -223 to 0.
	 *
	 * Example:
	 * Input of    0: 0.5 *    0 =   0.0dB [Max volume/No attenuation].
	 * Input of -103: 0.5 * -100 =  -51.5dB
	 * Input of -223: 0.5 * -223 = -111.5dB [Minumum volume/Max atenuation].
	 */
	void setAttenuation(volume_t attenuation)
	{
		setAttenuationLeft(attenuation);
		setAttenuationRight(attenuation);
	}

	void setAttenuationLeft(volume_t attenuation);
	void setAttenuationRight(volume_t attenuation);

	/**
	 * Direct gain control of the chip with 0.5dB precision.
	 *
	 * Gain can be set in the range 0dB to +31.5dB.
	 * Gain is calculated as: 0.5dB * gain.
	 * Input values goes from 0 to 63.
	 *
	 * Example:
	 * Input of  0: 0.5 *  0 =  0.0dB [Minumum gain.]
	 * Input of 31: 0.5 * 31 = 15.5dB
	 * Input of 63: 0.5 * 63 = 31.5dB [Maximum gain.]
	 */
	void setGain(volume_t gain)
	{
		setGainLeft(gain);
		setGainRight(gain);
	}

	void setGainLeft(volume_t gain);
	void setGainRight(volume_t gain);

	/**
	 * Mute channels.
	 * Muting is done using attenuation control data; which means that calling
	 * `setVolume` or `setAttenuation` is required to unmute.
	 */
	void mute()
	{
		muteLeft();
		muteRight();
	}

	void muteLeft();
	void muteRight();

	/**
	 * Enable or disable zero crossing. Defaults to enabled.
	 * Zero crossing only works if the zero crossing terminal pin is set low.
	 */
	void enableZeroCrossing();
	void disableZeroCrossing();
	bool isZeroCrossingEnabled() const { return state.zeroCrossing; }

	/**
	 * Enable or disable the L/R channel attenuation link.
	 */
	void enableAttenuationLink();
	void disableAttenuationLink();
	bool isAttenuationLinked() const { return state.linkAttenuation; }

	/**
	 * Enable or disable the L/R channel gain link.
	 */
	void enableGainLink();
	void disableGainLink();
	bool isGainLinked() const { return state.linkGain; }

private:
	void transferState();
	void transfer(address_t selectAddress, byte data);

private:
	address_t chipAddress;
	MusesDetails::StateControlData state;
};

#endif // INCLUDED_MUSES_72320
