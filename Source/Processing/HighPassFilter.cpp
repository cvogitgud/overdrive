/*
  ==============================================================================

    HighPassFilter.cpp
    Created: 7 Jul 2025 8:07:33pm
    Author:  Chris

  ==============================================================================
*/

#include "HighPassFilter.h"

HighPassFilter::HighPassFilter(){
    *filterProcessor.state = *Coefficients::makeHighPass(sampleRate, maxFreq, resonance);
}

void HighPassFilter::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels){
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    sampleRate = sampleRate;
    filterProcessor.prepare(spec);
    filterProcessor.reset();
}

void HighPassFilter::process (juce::AudioBuffer<float>& buffer){
    juce::dsp::AudioBlock<float> block {buffer};
    filterProcessor.process(juce::dsp::ProcessContextReplacing<float> (block));
}

void HighPassFilter::updateParameters (const float cutoff){
    *filterProcessor.state = *Coefficients::makeHighPass(sampleRate, cutoff, resonance);
}

void HighPassFilter::reset (){
    filterProcessor.reset();
}

