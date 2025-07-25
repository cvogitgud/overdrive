
<h1>Tube Schkreamer - A Basic Overdrive Plugin </h1>

This is my first personal plugin, and as a guitarist, of course it's an overdrive plugin! Built using the JUCE Framework in C++.
It uses the Schetzen Formula for symmetrical soft clipping, and follows the basic block diagram featured in [DSP Overdrive Algorithm in Software (STM32) - Phil's Lab #11](https://youtu.be/_0ys155xv1Q?si=TKS7VlCb2yQ-riOq).  


Essentially it's
1. Low pass filter (IIR) to clean up lows before distortion produces muddy lows
2. Anti-Aliasing using a fixed high pass FIR filter attenuate input frequencies above ~10kHz.
3. Distortion algorithm - Schetzen Formula
5. High pass filter (IIR) for harsh post-distortion high frequencies


<h2>Planned Changes</h2>
1. Make a dedicated Distortion module to practice dealing with types and modularity.
2. Add a bug fixes section to the README :)
