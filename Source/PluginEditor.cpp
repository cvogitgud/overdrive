/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), powerSwitch(audioProcessor.treeState, "POWER"), pregainDial(audioProcessor.treeState, "PREGAIN", "OVERDRIVE"), volumeDial(audioProcessor.treeState, "VOLUME", "LEVEL"), filterDial(audioProcessor.treeState, "LOWPASSCUTOFF", "TONE") 
{
    int width = 300;
    int height = width * 7/5;
    setSize (width, height);
    
    pedalLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    pedalLabel.setJustificationType(juce::Justification::centred);
    pedalLabel.setFont(juce::FontOptions(35.0f));
    pedalLabel.setText(pedalName, juce::dontSendNotification);
    
    addAndMakeVisible(pedalLabel);
    addAndMakeVisible(powerSwitch);
    addAndMakeVisible(pregainDial);
    addAndMakeVisible(filterDial);
    addAndMakeVisible(volumeDial);
}

OverdriveAudioProcessorEditor::~OverdriveAudioProcessorEditor()
{
}

//==============================================================================
void OverdriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0, 167, 116));
    
    float leftMargin = 20.0;
    float bottomMargin = 20.0;
    float width = getWidth() - leftMargin * 2;
    float height = getHeight() * 0.5;
    g.setColour(juce::Colours::white);
    
    g.drawRoundedRectangle(leftMargin, getBottom() - height - bottomMargin, width, height, 1, 2);
}

void OverdriveAudioProcessorEditor::resized()
{
    int xCoor = 0;
    int yCoor = 0;
    int dialWidth = getWidth() / 2;
    int dialHeight = getHeight() / 4 + 10;
    pregainDial.setBounds(xCoor, yCoor, dialWidth, dialHeight);
    volumeDial.setBounds(pregainDial.getRight(), yCoor, dialWidth, dialHeight);
    filterDial.setBounds(centerWithHorizontal(dialWidth), dialHeight - 50, dialWidth, dialHeight);
    
    int powerSwitchWidth = 120;
    int powerSwitchHeight = 80;
    int powerSwitchX = centerWithHorizontal(powerSwitchWidth);
    int powerSwitchY = getHeight() * 2.0f/3;
    powerSwitch.setBounds(powerSwitchX, powerSwitchY, powerSwitchWidth, powerSwitchHeight);
    
    int labelWidth = getWidth();
    int labelHeight = powerSwitch.getY() - filterDial.getBottom();
    int labelX = centerWithHorizontal(labelWidth);
    int labelY = filterDial.getBottom();
    pedalLabel.setBounds(labelX, labelY, labelWidth, labelHeight);
}

const int OverdriveAudioProcessorEditor::centerWithHorizontal(const int componentWidth){
    return getWidth() / 2 - componentWidth / 2;
}
