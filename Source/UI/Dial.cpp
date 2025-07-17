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
Dial::Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId, juce::String parameterName)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, parameterId, slider);
    dialName = parameterName;
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

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText (dialName, getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Dial::resized()
{
    // make this scale to the entire box bound
    int xCoor = 0;
    int yCoor = 0;
    int sliderWidth, sliderHeight;
    sliderWidth = sliderHeight = 75;
    
    slider.setBounds(xCoor, yCoor, sliderWidth, sliderHeight);
}
