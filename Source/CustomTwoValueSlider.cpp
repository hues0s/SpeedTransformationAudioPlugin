/*
  ==============================================================================

    CustomTwoValueSlider.cpp
    Created: 13 Jul 2021 1:24:10pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomTwoValueSlider.h"

//==============================================================================
CustomTwoValueSlider::CustomTwoValueSlider() {
    setSliderStyle (Slider::TwoValueHorizontal);
    setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    setLookAndFeel (&customLookAndFeel);
}

CustomTwoValueSlider::~CustomTwoValueSlider() { }

void CustomTwoValueSlider::init(double minRange, double maxRange, double intervalRange) {
    setRange(minRange, maxRange, intervalRange);
    setMinAndMaxValues(minRange, maxRange);
}
