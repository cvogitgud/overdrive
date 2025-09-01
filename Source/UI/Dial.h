/*
  ==============================================================================

    Dial.h
    Created: 26 Aug 2025 11:33:23am
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
        
        int diameter = juce::jmin(width, height) * 0.75;
        int radius = diameter / 2;
        int centreX = x + width / 2;
        int centreY = y + height / 2;
        int rX = centreX - radius;
        int rY = centreY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        g.setColour(juce::Colours::black);
        g.fillEllipse(rX, rY, diameter, diameter);
        
        juce::Path dialTickPath;
        float tickWidth = 5.0;
        float tickHeight = 10.0;
        juce::Rectangle tick = juce::Rectangle<float>(0.0 - tickWidth / 2, -radius, tickWidth, tickHeight);
        
        dialTickPath.addRoundedRectangle(tick, 1.5f);
        g.setColour(juce::Colours::white);
        g.fillPath(dialTickPath, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
        juce::Path dialEnds;
        int markRadius = 3;
        dialEnds.addEllipse(0.0 - markRadius / 2, -radius - 7, markRadius, markRadius);
        
        g.setColour(juce::Colours::white);
        g.fillPath(dialEnds, juce::AffineTransform::rotation(rotaryStartAngle).translated(centreX, centreY));
        g.fillPath(dialEnds, juce::AffineTransform::rotation(rotaryEndAngle).translated(centreX, centreY));
        
        float midAngle = rotaryStartAngle + (0.5 * (rotaryEndAngle - rotaryStartAngle));
        g.fillPath(dialEnds, juce::AffineTransform::rotation(midAngle).translated(centreX, centreY));
    }
};

class Dial  : public juce::Component
{
public:
    Dial(juce::AudioProcessorValueTreeState& treeState, juce::String parameterId, juce::String parameterName);
    ~Dial() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool hitTest(int x, int y) override;

private:
    DialLookAndFeel dialStyle;
    
    juce::Label label;
    void initLabel (const juce::String parameterName);
    
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    void initSlider (juce::AudioProcessorValueTreeState& treeState, juce::String parameterId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
