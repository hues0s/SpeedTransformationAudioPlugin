/*
  ==============================================================================

    CustomRotarySlider.cpp
    Created: 7 Jul 2021 5:08:38pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomRotarySlider.h"
#include "ProjectColours.h"

CustomRotarySlider::CustomRotarySlider() {
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    setColour(Slider::rotarySliderFillColourId, ProjectColours::mainColour);
    setLookAndFeel(&customLookAndFeel);
    setRotaryParameters(4.1f, 8.5f, true);
}

CustomRotarySlider::~CustomRotarySlider() { }

void CustomRotarySlider::mouseDrag(const MouseEvent& event) {
    Slider::mouseDrag(event);
    event.source.enableUnboundedMouseMovement(true);
}

void CustomRotarySlider::mouseUp(const MouseEvent& event) {
    Slider::mouseUp (event);
    Desktop::getInstance().getMainMouseSource().setScreenPosition (event.source.getLastMouseDownPosition());
}

void CustomRotarySlider::init(double minRange, double maxRange, double intervalRange, String title, String extraText) {
    setRange(minRange, maxRange, intervalRange);
    setValue(maxRange);
    setName(title.toUpperCase());
    setComponentID(extraText);
}
