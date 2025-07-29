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
    Filter(TubeSchkreamerEnums::FilterType type);
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateCutoff(const float cutoff);
    void reset();
    
private:
    int sampleRate {48000};
    float maxFreq {22000.0f};
    const float resonance {0.1f};
    TubeSchkreamerEnums::FilterType filterType;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filterProcessor;
    
};
