/*
  ==============================================================================

    FilterComponent.cpp
    Created: 7 Jul 2025 8:07:57pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    
    addAndMakeVisible(slider);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
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
    g.drawText ("Tone", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void FilterComponent::resized()
{
    // make this scale to the entire box bound
    int xCoor = 0;
    int yCoor = 0;
    int sliderWidth, sliderHeight;
    sliderWidth = sliderHeight = 75;
    
    slider.setBounds(xCoor, yCoor, sliderWidth, sliderHeight);
}
