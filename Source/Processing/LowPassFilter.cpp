/*
  ==============================================================================

    LowPassFilter.cpp
    Created: 8 Jul 2025 2:03:51pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(){
    *filterProcessor.state = *Coefficients::makeLowPass(sampleRate, maxFreq, resonance);
}

void LowPassFilter::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels){
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    sampleRate = sampleRate;
    filterProcessor.prepare(spec);
    filterProcessor.reset();
}

void LowPassFilter::process (juce::AudioBuffer<float>& buffer){
    juce::dsp::AudioBlock<float> block {buffer};
    filterProcessor.process(juce::dsp::ProcessContextReplacing<float> (block));
}

void LowPassFilter::updateParameters (const float cutoff){
    *filterProcessor.state = *Coefficients::makeLowPass(sampleRate, cutoff, resonance);
}

void LowPassFilter::reset (){
    filterProcessor.reset();
}
