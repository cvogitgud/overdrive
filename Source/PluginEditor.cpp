/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), pregainDial(audioProcessor.treeState, "PREGAIN"), filterDial(audioProcessor.treeState, "LOWPASSCUTOFF")
{
    int width = 300;
    int height = width * 7/5;
    setSize (width, height);
    
    addAndMakeVisible(pregainDial);
    addAndMakeVisible(filterDial);
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
    int filterDialWidth, filterDialHeight;
    
    // i want it to be half of the width so we can make the gain filterDial the other half
    filterDialWidth = filterDialHeight = getWidth() / 2;
    
    filterDial.setBounds(xCoor, yCoor, filterDialWidth, filterDialHeight);
}
