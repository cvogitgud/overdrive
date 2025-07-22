/*
  ==============================================================================

    Dial.cpp
    Created: 7 Jul 2025 8:07:57pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Dial.h"

//==============================================================================
Dial::Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterID, juce::String parameterName)
{
    this->parameterID = parameterID;
    this->parameterName = parameterName;
    
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, parameterID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::FontOptions(15.0f));
    label.setText(parameterName, juce::dontSendNotification);
    label.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::blue);
    
    addAndMakeVisible(label);
    addAndMakeVisible(slider);
}

Dial::~Dial()
{
}

void Dial::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Dial::resized()
{
    int sliderWidth, sliderHeight;
    sliderWidth = sliderHeight = getWidth() / 2;
    int sliderXPos = getWidth() / 2 - sliderWidth / 2;
    int sliderYPos = getHeight() / 3;
    
    int labelWidth = getWidth() / 2;
    int labelHeight = 40;
    int labelXPos = getWidth() / 2 - labelWidth / 2;
    int labelYPos = 0;

    slider.setBounds(sliderXPos, sliderYPos, sliderWidth, sliderHeight);
    
    // set label bounds
    label.setBounds(labelXPos, labelYPos, labelWidth, labelHeight);
}
