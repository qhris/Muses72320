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
	Muses.setZeroCrossing(true);     // Enable/Disable zero crossing.
	Muses.setAttenuationLink(false); // Left channel controls both L/R gain channel.
	Muses.setGainLink(false);        // Left channel controls both L/R attenuation channel.
}

void loop()
{
	CurrentVolume -= 1;
	if (CurrentVolume < -223) {
		CurrentVolume = 0;
	}

	Muses.setVolume(CurrentVolume);
	// Equivalent to 'Muses.setVolume(CurrentVolume, CurrentVolume)' for L/R ch.

	delay(10);
}
