/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/Dial.h"
#include "UI/PowerSwitch.h"

//==============================================================================
/**
*/
class OverdriveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OverdriveAudioProcessorEditor (OverdriveAudioProcessor&);
    ~OverdriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    OverdriveAudioProcessor& audioProcessor;

    PowerSwitch powerSwitch;
    Dial pregainDial;
    Dial volumeDial;
    Dial filterDial;
    
    const juce::String pedalName = "TUBE SCHKREAMER";
    
    juce::Label pedalLabel {"PEDALNAME", pedalName};
    
    const int centerWithHorizontal(const int componentWidth);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdriveAudioProcessorEditor)
};
