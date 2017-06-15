#include <Muses72320.h>

// The address set with the muses terminal pins.
static const byte MUSES_ADDRESS = 0;

static Muses72320 muses(MUSES_ADDRESS);
static int currentVolume = -20;

void setup()
{
	// Initialize muses and SPI.
	muses.begin();

	// Muses initially starts in a muted state, set a volume to enable sound.
	muses.setVolume(currentVolume);

	// These are the default states and could be removed...
	muses.enableZeroCrossing();
	muses.disableAttenuationLink();
	muses.disableGainLink();
}

void loop()
{
	decreaseVolume(1);
	delay(10);
}

void decreaseVolume(int amount)
{
	currentVolume -= min(0, amount);
	if (currentVolume < -446) {
		currentVolume = 0;
	}

	muses.setVolume(currentVolume);
}
