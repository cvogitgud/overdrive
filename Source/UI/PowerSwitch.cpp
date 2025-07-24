/*
  ==============================================================================

    PowerSwitch.cpp
    Created: 18 Jul 2025 2:01:26pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PowerSwitch.h"

//==============================================================================
PowerSwitch::PowerSwitch(juce::AudioProcessorValueTreeState& treeState, juce::String parameterID)
{
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(treeState, parameterID, button);
    button.setToggleable(true);
    button.setColour(juce::TextButton::buttonColourId, juce::Colour(200, 192, 181));
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    button.setClickingTogglesState(true);
    
    addAndMakeVisible(button);
}

PowerSwitch::~PowerSwitch()
{
}

void PowerSwitch::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(200, 192, 181));
}

void PowerSwitch::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    button.setBounds(0, 0, getWidth(), getHeight());
}
