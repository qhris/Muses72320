#ifndef MOCK_ARDUINO_HEADER
#define MOCK_ARDUINO_HEADER

#include <stdint.h>
#include <cassert>

#define HIGH 1
#define LOW 0

#define INPUT 0
#define OUTPUT 1

typedef uint8_t byte;

struct ArduinoMock
{
	static int pinModes[16];
	static byte pinValues[16];
};

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

inline void pinMode(int pin, int mode)
{
	assert(pin < 16);
	ArduinoMock::pinModes[pin] = mode;
}

inline void digitalWrite(int pin, byte value)
{
	assert(pin < 16);
	ArduinoMock::pinValues[pin] = value;
}

#endif // MOCK_ARDUINO_HEADER
