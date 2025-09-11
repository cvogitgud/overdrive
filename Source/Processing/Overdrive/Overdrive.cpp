/*
  ==============================================================================

    Overdrive.cpp
    Created: 5 Sep 2025 1:57:31pm
    Author:  Chris

  ==============================================================================
*/

#include "Overdrive.h"

void Overdrive::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels){
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    pregain.reset();
    pregain.prepare(spec);
    pregain.setRampDurationSeconds(0.02f);
    pregain.setGainLinear(1.0f);
}

void Overdrive::processBlock(juce::dsp::AudioBlock<float> block){
    pregain.process(juce::dsp::ProcessContextReplacing<float> (block));
    
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer (channel);

        for (int sample = 0; sample < block.getNumSamples(); ++sample){
            float input = channelData[sample];
            channelData[sample] = processSample(input);
        }
    }
}

float Overdrive::processSample(float input){
    float output = 0.0f;
    float absInput = std::fabs(input);
    float signInput = (input >= 0) ? 1.0f : -1.0f;
    float threshold = 1.0f/3.0f;

    if (absInput < threshold) {
        output = 2.0f * input;
    }
    else if (absInput >= threshold && absInput < (2.0f * threshold)){
        float base = 2.0f - 3.0f * absInput;
        output = signInput * (3.0f - (base * base)) / 3.0f;
    }
    else {
        output = signInput;
    }
    
    return output;
}

void Overdrive::setGain(float newGain){
    pregain.setGainLinear(newGain);
}
