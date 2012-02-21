# Retro TV exit effect #

This patch basically just adds a retro TV exit transition when you turn off your calculator.

See this video for the effect in action http://youtu.be/tRPs5Qo-Ei8

## Status ##

This only works for the TI-NSpire CX OS 3.1. Porting to other OS's and models should only require finding the right hooking addresses.

Code is a one-shot piece and isn't very elegant right now.

## Limitations ##

The hooking address I found seems to only work when the USB cable is plugged in. When I have time, I'll look for an address that will work for both when USB is plugged in and on battery.

This basically means that the effect will only show when the USB cable is plugged in.

## Building ##

First clone or get an archive of this repo.

Then, building it is as simple as ```cd```ing into the directory and typing ```make``` at the prompt.

If you don't have a toolchain handy, there's a binary that's included with the source code but it may not be kept up to date.

## Licence ##

Code is licensed to you under GPLv3. See LICENSE

## *sigh* Why did you make this? ##

It looks cool. I like to code for the NSpire. I was bored.