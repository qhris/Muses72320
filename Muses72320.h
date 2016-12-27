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

class Muses72320
{
public:
	// contextual data types.
	typedef uint8_t pin_t;
	typedef uint8_t data_t;
	typedef int16_t volume_t;
	typedef uint8_t address_t;

	// specify a connection over an address using three output pins.
	Muses72320(address_t chip_address);

	// set the pins in their correct states.
	void begin();

	// set the volume using the following formula:
	//   (-0.5 * volume) db
	// audio level goes from [-111.5, 0.0] dB
	// input goes from -223 to 0.
	void setVolume(volume_t left, volume_t right);
	inline void setVolume(volume_t volume) { setVolume(volume, volume); }

	// gain is constrained to [0, 31.5].
	// input goes from 0 to 63.
	void setGain(volume_t left, volume_t right);
	inline void setGain(volume_t volume) { setGain(volume, volume); }

  void mute();

	// enable or disable zero crossing.
	// enabling zero crossing only works if the zero crossing terminal is set low.
	void setZeroCrossing(bool enabled);
	void setAttenuationLink(bool enabled);
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
