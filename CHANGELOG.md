## Muses72320 Version 1.0.0 (In progress)

Muses72320 official 1.0 release.

### Breaking changes

* `setVolume(volume)` now controls volume in `0.25dB` steps (from `0.5dB`) using the `0.25dB`
gain step. This function allows the range `-111.5dB` to `0dB`. Allowed input values range from -446 to 0.
* `setVolume(lch, rch)` removed. Use `setVolumeLeft(v)` or `setVolumeRight(v)` instead.
* `setGain(lch, rch)` removed. Use `setGainLeft(g)` or `setGainRight(g)` instead.
* `setZeroCrossing(bool)` replaced with `enableZeroCrossing()` and `disableZeroCrossing`.
* `setAttenuationLink(bool)` replaced with `enableAttenuationLink()` and `disableAttenuationLink()`.
* `setGainLink(bool)` replaced with `enableGainLink()` and `disableGainLink()`.

### Additions

* Added the `example` directory for quick access via the arduino IDE.
* Added `setAttenuation(a)`, `setAttenuationLeft(a)`, and `setAttenuationRight(a)`. They work as `setVolume` previously did; with `0.5dB` steps.
* Added `setVolumeLeft(v)` and `setVolumeRight(v)`.
* Added `setGainLeft(g)` and `setGainRight(g)`.
* Added `muteLeft` and `muteRight`.
* Added `isZeroCrossingEnabled()`, `isGainLinked()`, and `isAttenuationLinked()`.
