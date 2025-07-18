/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), pregainDial(audioProcessor.treeState, "PREGAIN", "Pre-gain"), filterDial(audioProcessor.treeState, "LOWPASSCUTOFF", "Tone"), volumeDial(audioProcessor.treeState, "VOLUME", "Volume")
{
    int width = 300;
    int height = width * 7/5;
    setSize (width, height);

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
    dialWidth = dialHeight = getWidth() / 2;
    
    filterDial.setBounds(xCoor, yCoor, dialWidth, dialHeight);
    pregainDial.setBounds(filterDial.getRight(), yCoor, dialWidth, dialHeight);
    volumeDial.setBounds(xCoor + dialWidth / 2, dialHeight, dialWidth, dialHeight);
}
