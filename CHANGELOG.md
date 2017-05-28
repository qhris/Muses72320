## Muses72320 Version 1.0.0 (In progress)

Muses72320 official 1.0 release.

### Additions

* Added the `example` directory for quick access via the arduino IDE.
* Added `setAttenuation(...)` to work as `setVolume` previously did, with `0.5dB` steps.
* Added `setFineGain(bool)` to control the `+0.25dB` step, so it can be used with `setAttenuation(...)` to get a range from `-111.5dB` to `+0.25dB`.
* Added `mute(bool, bool)` to allow muting channels independently.

### Breaking changes

* `setVolume(...)` now controls volume in `0.25dB` steps (from `0.5dB`) using the `0.25dB`
gain step. This function allows the range `-111.5dB` to `0dB`.
