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

class Muses72320
{
public:
	using data_t = MusesTypes::data_t;
	using volume_t = MusesTypes::volume_t;
	using address_t = MusesTypes::address_t;

	Muses72320(address_t chipAddress);

	/**
	 * Initializes the pin modes to enable communication with the chip.
	 * Uses digital pin 10 as the slave select pin and enables SPI.
	 */
	void begin();

	/**
	 * Simple attenuation control with 0.25dB steps utilizing the 0.25dB gain.
	 *
	 * Volume control range goes from -111.5dB to 0dB using this function.
	 * Output dB is calculated as: 0.25dB * volume.
	 * Input values goes from -446 to 0.
	 *
	 * Example:
	 * Input of    0: -0.25 *   0 =    0.00dB [Max volume without gain]
	 * Input of -105: -0.25 * 105 =  -26.25dB
	 * Input of -446: -0.25 * 446 = -111.50dB [Minimum volume]
	 */
	void setVolume(volume_t left, volume_t right);
	void setVolume(volume_t volume) { setVolume(volume, volume); }

	/**
	 * Direct attenuation control of the chip only allowing 0.5dB steps.
	 *
	 * Attenuation control ranges from -111.5 to 0dB using this function.
	 * Output dB is calculated as: 0.5dB * attenuation.
	 * Input values goes from -223 to 0.
	 *
	 * Example:
	 * Input of    0: 0.5 *    0 =   0.0dB [Max volume/No attenuation].
	 * Input of -100: 0.5 * -100 =  -50.0dB
	 * Input of -103: 0.5 * -100 =  -51.5dB
	 * Input of -223: 0.5 * -223 = -111.5dB [Minumum volume/Max atenuation].
	 */
	void setAttenuation(volume_t left, volume_t right);
	void setAttenuation(volume_t attenuation)
		{ setAttenuation(attenuation, attenuation); }

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
	void setGain(volume_t left, volume_t right);
	void setGain(volume_t volume) { setGain(volume, volume); }

	/**
	 * Mutes the left or right channel.
	 * Call setGain(), setAttenuation() or setVolume() to unmute.
	 */
	void mute(bool left, bool right);
	void mute() { mute(true, true); }

	/**
	 * Enable or disable zero crossing. Defaults to enabled.
	 * Zero crossing only works if the zero crossing terminal pin is set low.
	 */
	void setZeroCrossing(bool enabled);

	/**
	 * Enables or disables the attenuation link. Defaults to disabled.
	 *
	 * With attenuation link enabled, controlling the left channel attenuation
	 * also contols the right channel.
	 */
	void setAttenuationLink(bool enabled);

	/**
	 * Enables or disables the gain link. Defaults to disabled.
	 *
	 * With gain link enabled, controlling the left channel gain
	 * also contols the right channel.
	 */
	void setGainLink(bool enabled);

	bool isAttenuationLinked() const;
	bool isGainLinked() const;

private:
	void setVolumeLeft(volume_t volume);
	void setVolumeRight(volume_t volume);
	void setAttenuationLeft(volume_t attenuation);
	void setAttenuationRight(volume_t attenuation);
	void setGainLeft(volume_t gain);
	void setGainRight(volume_t gain);
	void muteLeft();
	void muteRight();

	void transfer(address_t selectAddress, data_t data);

private:
	address_t chipAddress;
	data_t states;
};

#endif // INCLUDED_MUSES_72320
