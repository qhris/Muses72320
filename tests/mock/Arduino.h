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

inline byte digitalRead(int pin)
{
	assert(pin < 16);
	return ArduinoMock::pinValues[pin];
}

inline int mockReadPinMode(int pin)
{
	assert(pin < 16);
	return ArduinoMock::pinModes[pin];
}

// Pins are normally defined in pins_arduino.h
#define PIN_SPI_SS (10)
#define PIN_SPI_MOSI (11)
#define PIN_SPI_MISO (12)
#define PIN_SPI_SCK (13)

static const uint8_t SS = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

#endif // MOCK_ARDUINO_HEADER
