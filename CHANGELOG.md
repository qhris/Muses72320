## Muses72320 Version 1.0.0 (In progress)

Muses72320 official 1.0 release.

### Breaking changes

* `setVolume(volume)` now controls volume in `0.25dB` steps (from `0.5dB`) using the `0.25dB`
gain step. This function allows the range `-111.5dB` to `0dB`. Allowed input values range from -446 to 0.
* `setVolume(lch, rch)` removed. Use `setVolumeLeft` or `setVolumeRight` instead.
* `setGain(lch, rch)` removed. Use `setGainLeft` or `setGainRight` instead.

### Additions

* Added the `example` directory for quick access via the arduino IDE.
* Added `setAttenuation`, `setAttenuationLeft`, and `setAttenuationRight`. They work as `setVolume` previously did; with `0.5dB` steps.
* Added `setVolumeLeft` and `setVolumeRight`.
* Added `setGainLeft` and `setGainRight`.
* Added `muteLeft` and `muteRight`.
