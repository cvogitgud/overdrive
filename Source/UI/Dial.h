/*
  ==============================================================================

    Dial.h
    Created: 7 Jul 2025 8:07:57pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Dial  : public juce::Component
{
public:
    Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId, juce::String parameterName);
    ~Dial() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    
    // label
    juce::String dialName;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
