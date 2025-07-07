
<h1>Basic Overdrive Plugin - Sunset</h1>

This is my first personal plugin, and as a guitarist, of course it's an overdrive plugin!
It follows the basic block diagram featured in [DSP Overdrive Algorithm in Software (STM32) - Phil's Lab #11](https://youtu.be/_0ys155xv1Q?si=TKS7VlCb2yQ-riOq).  

Essentially it's
1. Low pass filter (IIR) to clean up lows before distortion produces muddy lows
2. Oversampling (Anti-aliasing)
3. Distortion algorithm
4. Downsampling (Anti-aliasing)
5. High pass filter (IIR) for harsh post-distortion high frequencies

I'll be updating this README as necessary as I make updates.
