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
    addAndMakeVisible(button);
}

PowerSwitch::~PowerSwitch()
{
}

void PowerSwitch::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PowerSwitch", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PowerSwitch::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    button.centreWithSize(50, 50);
}
