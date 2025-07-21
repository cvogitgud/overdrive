/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), powerSwitch(audioProcessor.treeState, "POWER"), pregainDial(audioProcessor.treeState, "PREGAIN", "Pre-gain"), volumeDial(audioProcessor.treeState, "VOLUME", "Volume"), filterDial(audioProcessor.treeState, "LOWPASSCUTOFF", "Tone") 
{
    int width = 300;
    int height = width * 7/5;
    setSize (width, height);
    
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Overdrive", getLocalBounds(), juce::Justification::centred, 1);
}

void OverdriveAudioProcessorEditor::resized()
{
    int xCoor = 0;
    int yCoor = 0;
    int dialWidth, dialHeight;
    dialWidth = getWidth() / 2;
    dialHeight = getHeight() / 4;
    
    int powerSwitchWidth, powerSwitchHeight;
    powerSwitchWidth = 100;
    powerSwitchHeight = 100;
    
    // set powerSwitch component bounds
    
    powerSwitch.setBounds(getWidth() / 2 - powerSwitchWidth / 2, getHeight() * 2/3.0f, powerSwitchWidth, powerSwitchHeight);
    pregainDial.setBounds(xCoor, yCoor, dialWidth, dialHeight);
    volumeDial.setBounds(pregainDial.getRight(), yCoor, dialWidth, dialHeight);
    filterDial.setBounds(xCoor + dialWidth / 2, dialHeight, dialWidth, dialHeight);
}
