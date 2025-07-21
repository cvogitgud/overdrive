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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OverdriveAudioProcessor& audioProcessor;

    PowerSwitch powerSwitch;
    Dial pregainDial;
    Dial volumeDial;
    Dial filterDial;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdriveAudioProcessorEditor)
};
