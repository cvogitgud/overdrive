/*
  ==============================================================================

    HighPassFilter.h
    Created: 7 Jul 2025 8:07:33pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class HighPassFilter {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(float cutoff, float resonance);
    void reset();
    
private:
    int sampleRate {40000};
    float maxFreq {20000.0f};
    
    // ugly and confusing to have the filter as a ProcessorDuplicator type under the hood
    // makes it confusing and difficult to work with if trying to use it as a filter
    // but then only have access to a member variable called "state".
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilterProcessor {juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, maxFreq, 0.1f)};
    
    
};
