/*
  ==============================================================================

    Dial.h
    Created: 7 Jul 2025 8:07:57pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/

class DialLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override {
        
        int diameter = juce::jmin(width, height);
        int radius = diameter / 2;
        int centreX = x + width / 2;
        int centreY = y + height / 2;
        float angle = rotaryStartAngle + (sliderPos * (rotaryStartAngle - rotaryEndAngle));
        
        g.setColour(juce::Colours::red);
        g.drawRect(x, y, width, height);
        g.fillEllipse(x, y, width, height);
        
        juce::Path dialTickPath;
        dialTickPath.addRectangle(0, -radius, 10.0f, radius);
        g.setColour(juce::Colours::black);
        g.fillPath(dialTickPath, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
    }
};


class Dial  : public juce::Component
{
public:
    Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId, juce::String parameterName);
    ~Dial() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DialLookAndFeel dialStyle;
    
    juce::Label label;
    void initLabel (const juce::String parameterName);
    
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    void initSlider (juce::AudioProcessorValueTreeState& treeState, juce::String parameterId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
