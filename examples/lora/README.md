LoRa
====

LoRa (Long Range) is a radio communication technique that uses
spread spectrum modulation technique derived from chirp spread
spectrum (CSS) technology.

LoRa provides a number of properties including:

 * long range, can cover tens of kilometres
 * low power, devices can run for years
 * reasonably secure
 * standardised
 * relativity cheap

This directory contains a range of examples using
[RadioLib](https://github.com/jgromes/RadioLib) to
support LoRa on Tock.

Before the examples will work you first need to build
RadioLib.

Note that the Makefiles will do this automatically when
you run `make` in a subdirectory, but if you want to do
it manually you can run the `build-RadioLib.sh` script.
