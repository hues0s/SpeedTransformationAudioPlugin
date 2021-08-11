/*
  ==============================================================================

    CustomTwoValueSlider.h
    Created: 13 Jul 2021 1:24:10pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LookAndFeelCustomTwoValueSlider.h"


class CustomTwoValueSlider : public Slider {
public:
    CustomTwoValueSlider();
    ~CustomTwoValueSlider() override;
    
    void init(double minRange, double MaxRange, double intervalRange, String title);

private:
    LookAndFeelCustomTwoValueSlider customLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomTwoValueSlider)
};
