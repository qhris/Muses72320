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

/**
 * The Muses72320 IC.
 *
 * Allows communication with the chip over I2C.
 */
class Muses72320
{
public:
	// contextual data types.
	typedef uint8_t data_t;
	typedef int16_t volume_t;
	typedef uint8_t address_t;

	/**
	 * Creates a new chip which can be controlled on a specific chip address.
	 */
	Muses72320(address_t chip_address);

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
	 *
	 * @param left  Left channel volume. Range: [-446, 0].
	 * @param right Right channel volume. Range: [-446, 0].
	 */
	void setVolume(volume_t left, volume_t right);

	/**
	 * Simple attenuation control with 0.25dB steps utilizing the 0.25dB gain.
	 *
	 * Volume control ranges from -111.5dB to 0dB using this function.
	 * Output dB is calculated as: 0.25dB * volume.
	 * Input values goes from -446 to 0.
	 *
	 * Example:
	 * Input of    0: -0.25 *   0 =    0.00dB [Max volume without gain]
	 * Input of -105: -0.25 * 105 =  -26.25dB
	 * Input of -446: -0.25 * 446 = -111.50dB [Minimum volume]
	 *
	 * @param volume Left and right channel volume. Range: [-446, 0].
	 */
	inline void setVolume(volume_t volume) { setVolume(volume, volume); }

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
	 *
	 * @param left  Left channel attenuation. Range: [-223, 0].
	 * @param right Right channel attenuation. Range: [-223, 0].
	 */
	void setAttenuation(volume_t left, volume_t right);

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
	 *
	 * @param left  Left channel attenuation. Range: [-223, 0].
	 * @param right Right channel attenuation. Range: [-223, 0].
	 */
	inline void setAttenuation(volume_t attenuation)
		{ setAttenuation(attenuation, attenuation); }

	/**
	 * Sets the fine gain control bit, adding 0.25dB to the attenuation if set.
	 *
	 * Note that this will not work with gain, because setting sets the gain
	 * to 0.25dB, no matter what value was set before with setGain().
	 * For use with setAttenuation().
	 *
	 * @param enabled Sets gain to 0.25dB if set, or to 0dB if not.
	 */
	void setFineGain(bool enabled)

	// gain is constrained to [0, 31.5].
	// input goes from 0 to 63.

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
	 *
	 * @param left  Left channel gain control. Range: [0, 63].
	 * @param right Right channel gain control. Range: [0, 63].
	 */
	void setGain(volume_t left, volume_t right);

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
	 *
	 * @param gain Left and right channel gain control. Range: [0, 63].
	 */
	inline void setGain(volume_t volume) { setGain(volume, volume); }

	/**
	 * Mutes the left or right channel.
	 * Call setGain(), setAttenuation() or setVolume() to unmute.
	 *
	 * @param left Mutes the left channel if set.
	 * @param right Mutes the right channel if set.
	 */
	void mute(bool left, bool right);

	/**
	 * Mutes the left and right channel.
	 * Call setGain(), setAttenuation() or setVolume() to unmute.
	 */
	inline void mute() { mute(true, true); }

	/**
	 * Enable or disable zero crossing. Defaults to enabled.
	 *
	 * Zero crossing only works if the zero crossing terminal pin is set low.
	 *
	 * @param enabled Enable or disable zero crossing.
	 */
	void setZeroCrossing(bool enabled);

	/**
	 * Enables or disables the attenuation link. Defaults to disabled.
	 *
	 * With attenuation link enabled, controlling the left channel attenuation
	 * also contols the right channel.
	 * Without attenuation link set the left and right channel are controlled
	 * independently.
	 *
	 * @param enabled Enables or disables attenuation link.
	 */
	void setAttenuationLink(bool enabled);

	/**
	 * Enables or disables the gain link. Defaults to disabled.
	 *
	 * With gain link enabled, controlling the left channel gain
	 * also contols the right channel.
	 * Without gain link set the left and right channel are controlled
	 * independently.
	 *
	 * @param enabled Enables or disables gain link.
	 */
	void setGainLink(bool enabled);

private:
	void transfer(address_t address, data_t data);

private:
	// for multiple chips on the same bus line.
	address_t chip_address;

	// muses state bits:
	//	 7:     link l/r attenuation
	//   6:     link l/r gain
	//   5:     disable zero crossing
	//   [4-0]: not used
	data_t states;
};

#endif // INCLUDED_MUSES_72320
