/*
  ==============================================================================

    Dial.cpp
    Created: 26 Aug 2025 11:33:23am
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Dial.h"

//==============================================================================
Dial::Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId, juce::String parameterName)
{
    initLabel(parameterName);
    addAndMakeVisible(label);
    
    initSlider(treeState, parameterId);
    slider.setLookAndFeel(&dialStyle);
    addAndMakeVisible(slider);
}

Dial::~Dial()
{
}

void Dial::initSlider(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId){
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, parameterId, slider);
}

void Dial::initLabel(const juce::String parameterName){
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::FontOptions(12.0f));
    label.setText(parameterName, juce::dontSendNotification);
}

void Dial::paint (juce::Graphics& g)
{
}

void Dial::resized()
{
    int labelWidth = getWidth() / 2;
    int labelHeight = getHeight() * 0.25;
    int labelX = getWidth() / 2 - labelWidth / 2;
    int labelY = 0;
    label.setBounds(labelX, labelY, labelWidth, labelHeight);
    
    int sliderWidth, sliderHeight;
    sliderWidth = sliderHeight = getHeight() * 0.65 * 1.25;
    int sliderX = getWidth() / 2 - sliderWidth / 2;
    int sliderY = label.getBottom();
    
    slider.setBounds(sliderX, sliderY, sliderWidth, sliderHeight);
}

bool Dial::hitTest(int x, int y) {
    bool withinSliderHorizontal = slider.getX() <= x && x <= slider.getRight();
    bool withinSliderVertical = slider.getY() <= y && y <= slider.getBottom();
    return withinSliderHorizontal && withinSliderVertical;
}
