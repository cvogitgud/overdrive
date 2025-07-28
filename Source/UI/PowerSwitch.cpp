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
    button.setColour(juce::TextButton::buttonColourId, switchColour);
    button.setColour(juce::TextButton::buttonOnColourId, switchColour);
    button.setClickingTogglesState(true);
    
    addAndMakeVisible(button);
}

PowerSwitch::~PowerSwitch()
{
}

juce::TextButton& PowerSwitch::getButton(){
    return button;
}

void PowerSwitch::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(200, 192, 181));
}

void PowerSwitch::resized()
{
    button.setBounds(0, 0, getWidth(), getHeight());
}
