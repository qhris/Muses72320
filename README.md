# Muses 72320

Arduino library for communicating with the Muses 72320 volume control IC.

The data sheets can be found [here](http://www.njr.com/semicon/PDF/MUSES72320_E.pdf) (pdf).

## Download

Download the latest release over at the [Releases](https://github.com/qhris/Muses72320/releases) page.

## Example

```c++
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
```

## Problems

Please post any problems on the [Issues](https://github.com/qhris/Muses72320/issues) page.

## License

MIT. Please read over the LICENSE file included in the project.
