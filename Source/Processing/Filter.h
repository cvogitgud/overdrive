/*
  ==============================================================================

    Filter.h
    Created: 17 Jul 2025 5:36:56pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Enums.h"

class Filter {
public:
    Filter(OverdriveEnums::FilterType type);
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const float cutoff);
    void reset();
    
private:
    int sampleRate {48000};
    float maxFreq {22000.0f};
    const float resonance {0.1f};
    OverdriveEnums::FilterType filterType;
    
    // ugly and confusing to have the filter as a ProcessorDuplicator type under the hood
    // makes it confusing and difficult to work with if trying to use it as a filter
    // but then only have access to a member variable called "state".
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filterProcessor;
    
};
