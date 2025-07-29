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
#include "UI/PowerLED.h"

//==============================================================================
/**
*/
class TubeSchkreamerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TubeSchkreamerAudioProcessorEditor (TubeSchkreamerAudioProcessor&);
    ~TubeSchkreamerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void togglePowerLED();

private:
    TubeSchkreamerAudioProcessor& audioProcessor;

    PowerSwitch powerSwitch;
    Dial pregainDial;
    Dial volumeDial;
    Dial filterDial;
    
    const juce::String pedalName = "TUBE SCHKREAMER";
    juce::Label pedalLabel {"PEDALNAME", pedalName};
    
    const int centerWithHorizontal(const int componentWidth);
    
    PowerLED powerLED;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TubeSchkreamerAudioProcessorEditor)
};
