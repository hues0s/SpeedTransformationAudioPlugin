/*
  ==============================================================================

    CustomRotarySlider.h
    Created: 7 Jul 2021 5:08:38pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LookAndFeelCustomRotarySlider.h"


class CustomRotarySlider : public Slider {

public:
    CustomRotarySlider();
    ~CustomRotarySlider() override;
    
    void mouseDrag(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;
    
    void init(double minRange, double maxRange, double intervalRange, String title, String extraText);
    
private:
    LookAndFeelCustomRotarySlider customLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomRotarySlider)
};
