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
    initSlider(treeState, parameterId);
    slider.setLookAndFeel(&dialStyle);
    addAndMakeVisible(slider);
    
    initLabel(parameterName);
    addAndMakeVisible(label);
}

Dial::~Dial()
{
}

void Dial::initSlider(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId){
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, parameterId, slider);
}

void Dial::initLabel(const juce::String parameterName){
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::FontOptions(15.0f));
    label.setText(parameterName, juce::dontSendNotification);
}

void Dial::paint (juce::Graphics& g)
{
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//    g.fillAll(juce::Colour(0, 167, 116));
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
