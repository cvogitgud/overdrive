/*
  ==============================================================================

    PowerSwitch.h
    Created: 18 Jul 2025 2:01:26pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PowerSwitch  : public juce::Component
{
public:
    PowerSwitch(juce::AudioProcessorValueTreeState& treeState, juce::String parameterID);
    ~PowerSwitch() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::ToggleButton button;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PowerSwitch)
};
