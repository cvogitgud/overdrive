
<h1>Tube Schkreamer - A Basic Overdrive Plugin </h1>

This is my first personal plugin, and as a guitarist, of course it's an overdrive plugin! Built using the JUCE Framework in C++.
It uses the Schetzen Formula for symmetrical soft clipping, and follows the basic block diagram featured in [DSP Overdrive Algorithm in Software (STM32) - Phil's Lab #11](https://youtu.be/_0ys155xv1Q?si=TKS7VlCb2yQ-riOq).


The block diagram is essentially
1. Low pass filter (IIR) to clean up lows before distortion produces muddy lows
2. Anti-Aliasing using a fixed high pass FIR filter to attenuate input frequencies above ~10kHz
3. Distortion algorithm - Schetzen Formula
5. High pass filter (IIR) for harsh post-distortion high frequencies


*New package installer coming*

Happy playing!


<h2>Contact</h2>
I'd love any and all feedback as I'll be maintaining this pet project of mine. Contact me at cvo13014@gmail.com. New releases will focus on the UI, but I'm interested in trying out new Distortion DSP algorithms. Look out for my next project - a delay pedal!

<h2>Releases</h2>
<h3>Version 1.0</h3>
<ul>
  <li>Schetzen Formula for distortion</li>
  <li>Linearly scaled dials (values are not skewed, e.g. lower range of values occupying more of the dial than higher range)</li>
  <li>Floating point values for pregain and volume, rather than decibels</li>
</ul>
