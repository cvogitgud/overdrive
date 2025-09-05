/*
  ==============================================================================

    Overdrive.h
    Created: 5 Sep 2025 1:57:31pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Overdrive {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void processBlock(juce::dsp::AudioBlock<float> block);
    float processSample(float input);
    
    void setGain(float gain);
    
private:
    juce::dsp::Gain<float> pregain;
};
