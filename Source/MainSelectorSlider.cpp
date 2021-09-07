/*
  ==============================================================================

    MainSelectorSlider.cpp
    Created: 31 Jul 2021 1:53:10pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "MainSelectorSlider.h"
#include <JuceHeader.h>
#include "ProjectColours.h"

MainSelectorSlider::MainSelectorSlider() {
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    setColour(Slider::rotarySliderFillColourId, ProjectColours::mainColour);
    setLookAndFeel(&customLookAndFeel);
    setRotaryParameters(5.3f, 10.42f - (10.4f - 5.28f)/9, true);

    setRange(0, 7, 1);
    setValue(4);
    
}

MainSelectorSlider::~MainSelectorSlider() { }

void MainSelectorSlider::mouseDrag(const MouseEvent& event) {
    Slider::mouseDrag(event);
    event.source.enableUnboundedMouseMovement(true);
}

void MainSelectorSlider::mouseUp(const MouseEvent& event) {
    Slider::mouseUp (event);
    Desktop::getInstance().getMainMouseSource().setScreenPosition (event.source.getLastMouseDownPosition());
}
