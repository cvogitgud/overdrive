/*
  ==============================================================================

    HighPassFilter.cpp
    Created: 7 Jul 2025 8:07:33pm
    Author:  Chris

  ==============================================================================
*/

#include "HighPassFilter.h"

void HighPassFilter::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels){
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    highPassFilterProcessor.prepare(spec);
    highPassFilterProcessor.reset();
}

void HighPassFilter::process (juce::AudioBuffer<float>& buffer){
    juce::dsp::AudioBlock<float> block {buffer};
    highPassFilterProcessor.process(juce::dsp::ProcessContextReplacing<float> (block));
}

void HighPassFilter::updateParameters (float cutoff, float resonance){
    *highPassFilterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutoff, resonance);
}

void HighPassFilter::reset (){
    highPassFilterProcessor.reset();
}
