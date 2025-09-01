/*
  ==============================================================================

    PowerLED.h
    Created: 26 Aug 2025 11:44:20am
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PowerLED  : public juce::Component
{
public:
    PowerLED(juce::Colour colour);
    ~PowerLED() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void toggleOn();
    void toggleOff();
    void setLEDColour(juce::Colour colour);
    void setRadius(float radius);
    
    bool hitTest(int x, int y) override;

private:
    float radius = 10.0f;
    float width;
    float height;
    juce::Colour ledColour;
    
    bool isOn = true;
    
    void createGlow(juce::Graphics& g);
    void drawCenteredCircle(juce::Graphics& g, float radius);
    float centerHorizontal(float containerWidth, float width);
    float centerVertical(float containerHeight, float height);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PowerLED)
};
