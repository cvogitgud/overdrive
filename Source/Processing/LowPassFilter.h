/*
  ==============================================================================

    LowPassFilter.h
    Created: 8 Jul 2025 2:03:51pm
    Author:  Chris

  ==============================================================================
*/

#pragma once
#include "LowPassFilter.h"

class LowPassFilter {
public:
    LowPassFilter();
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const float cutoff);
    void reset();
    
private:
    int sampleRate {48000};
    float maxFreq {22000.0f};
    const float resonance {0.1f};
    
    // ugly and confusing to have the filter as a ProcessorDuplicator type under the hood
    // makes it confusing and difficult to work with if trying to use it as a filter
    // but then only have access to a member variable called "state".
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorDuplicator<Filter, Coefficients> filterProcessor;
    
};
