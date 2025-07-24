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
    pedalLabel.setFont(juce::FontOptions(20.0f));
    pedalLabel.setText(pedalName, juce::dontSendNotification);
//    pedalLabel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0, 167, 116));
    
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour(0, 167, 116));

}

void OverdriveAudioProcessorEditor::resized()
{
    int xCoor = 0;
    int yCoor = 0;
    int dialWidth, dialHeight;
    dialWidth = getWidth() / 2;
    dialHeight = getHeight() / 4 + 10;
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
