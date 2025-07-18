/*
  ==============================================================================

    Filter.cpp
    Created: 17 Jul 2025 5:36:56pm
    Author:  Chris

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter(OverdriveEnums::FilterType type){
    filterType = type;
    switch (filterType){
        case OverdriveEnums::FilterType::Lowpass:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, maxFreq, resonance);
            break;
        case OverdriveEnums::FilterType::Highpass:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, maxFreq, resonance);
            break;
        default:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, maxFreq, resonance);
            break;
    }
}

void Filter::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels){
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    sampleRate = sampleRate;
    filterProcessor.prepare(spec);
    filterProcessor.reset();
}

void Filter::process (juce::AudioBuffer<float>& buffer){
    juce::dsp::AudioBlock<float> block {buffer};
    filterProcessor.process(juce::dsp::ProcessContextReplacing<float> (block));
}

void Filter::updateParameters (const float cutoff){
    switch (filterType){
        case OverdriveEnums::FilterType::Lowpass:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, resonance);
            break;
        case OverdriveEnums::FilterType::Highpass:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutoff, resonance);
            break;
        default:
            *filterProcessor.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, resonance);
            break;
    }
}

void Filter::reset (){
    filterProcessor.reset();
}
