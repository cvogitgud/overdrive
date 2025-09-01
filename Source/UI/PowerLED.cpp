/*
  ==============================================================================

    PowerLED.cpp
    Created: 26 Aug 2025 11:44:20am
    Author:  Chris

  ==============================================================================
*/


#include <JuceHeader.h>
#include "PowerLED.h"

//==============================================================================
PowerLED::PowerLED(juce::Colour colour)
{
    this->ledColour = colour;
}

PowerLED::~PowerLED()
{
}

void PowerLED::setLEDColour(juce::Colour colour){
    this->ledColour = colour;
}

void PowerLED::setRadius(float radius){
    this->radius = radius;
}

void PowerLED::toggleOn(){
    this->isOn = true;
    this->repaint();
}

void PowerLED::toggleOff(){
    this->isOn = false;
    this->repaint();
}

void PowerLED::paint (juce::Graphics& g)
{
    this->width = getWidth();
    this->height = getHeight();
    
    float alpha = 0.95f;
    if (this->isOn){
        // TODO: Adjust glow effect on green, looks terrible!
        // createGlow(g);
    }
    else {
        alpha = 0.5f;
    }
    
    // LED Border
    g.setColour(juce::Colours::black.withAlpha(0.75f));
    drawCenteredCircle(g, this->radius + 1.25f);
    
    // LED
    g.setColour(ledColour.withAlpha(alpha));
    drawCenteredCircle(g, this->radius);
}

void PowerLED::createGlow(juce::Graphics& g){
    float tempRadius = this->width;
    
    while (tempRadius > radius){
        tempRadius *= 0.85f;
        float alpha = 1 / juce::square(tempRadius / this->radius) * 0.5;
        g.setColour(ledColour.withAlpha(alpha));
        drawCenteredCircle(g, tempRadius);
    }
}

void PowerLED::drawCenteredCircle(juce::Graphics& g, float radius){
    float centerX = centerHorizontal(this->width, radius);
    float centerY = centerVertical(this->height, radius);
    
    g.fillEllipse(centerX, centerY, radius, radius);
}

void PowerLED::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool PowerLED::hitTest(int x, int y){
    bool inXBounds = (0 <= x && x <= radius);
    bool inYBounds = (0 <= y && y <= radius);
    return inXBounds && inYBounds;
}

float PowerLED::centerHorizontal(float containerWidth, float width){
    return containerWidth / 2 - width / 2;
}

float PowerLED::centerVertical(float containerHeight, float height){
    return containerHeight / 2 - height / 2;
}
