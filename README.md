# Muses 72320

Arduino library for communicating with the Muses 72320 volume control IC.

The data sheets can be found [here](http://www.njr.com/semicon/PDF/MUSES72320_E.pdf) (pdf).

## Download

Download the latest release over at the [Releases](https://github.com/qhris/Muses72320/releases) page.

## Example

```c++
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
  if (CurrentVolume < -446) {
    CurrentVolume = 0;
  }

  Muses.setVolume(CurrentVolume);

  delay(10);
}

```

## Problems

Please post any problems on the [Issues](https://github.com/qhris/Muses72320/issues) page.

## License

MIT. Please read over the LICENSE file included in the project.
