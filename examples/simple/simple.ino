#include <Muses72320.h>

// The address wired into the muses chip (usually 0).
static const byte MUSES_ADDRESS = 0;

static Muses72320 Muses(MUSES_ADDRESS);
static Muses72320::volume_t CurrentVolume = -20;

void setup()
{
	// Initialize muses (SPI, pin modes)...
	Muses.begin();

	// Muses initially starts in a muted state, set a volume to enable sound.
	Muses.setVolume(CurrentVolume);

	// These are the default states and could be removed...
	Muses.enableZeroCrossing();
	Muses.disableAttenuationLink();
	Muses.disableGainLink();
}

void loop()
{
	CurrentVolume -= 1;
	if (CurrentVolume < -446) {
		CurrentVolume = 0;
	}

	Muses.setVolume(CurrentVolume);

	delay(10);
}
