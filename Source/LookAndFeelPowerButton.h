/*
  ==============================================================================

    LookAndFeelPowerButton.h
    Created: 25 Jul 2021 11:05:02am
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class LookAndFeelPowerButton : public LookAndFeel_V4 {

public:
    void drawTickBox (Graphics & g, Component & component, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
private:
    Colour customGrey = Colour::fromFloatRGBA (0.2, 0.2, 0.2, 0.8);
};
