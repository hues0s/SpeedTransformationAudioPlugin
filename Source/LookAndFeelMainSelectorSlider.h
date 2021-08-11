/*
  ==============================================================================

    LookAndFeelMainSelectorSlider.h
    Created: 31 Jul 2021 1:58:45pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class LookAndFeelMainSelectorSlider : public LookAndFeel_V4 {

public:
    void drawRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider&) override;
    
private:
    Colour customWhite = Colour::fromFloatRGBA (0.831373, 0.835294, 0.890196, 1.0);
    Colour customGrey = Colour::fromFloatRGBA (0.2, 0.2, 0.2, 0.8);
    Colour interiorEllipseColour = Colour::fromRGBA(63, 63, 63, 255);
};
