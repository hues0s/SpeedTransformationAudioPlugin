/*
  ==============================================================================

    LookAndFeelCustomTwoValueSlider.h
    Created: 13 Jul 2021 1:24:58pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class LookAndFeelCustomTwoValueSlider : public LookAndFeel_V4 {

public:
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;
    Slider::SliderLayout getSliderLayout(Slider& slider) override;
    
private:
    Colour blue = Colour::fromFloatRGBA (0.42, 0.83, 1.0, 1.0);
    Colour stYellow = Colour::fromFloatRGBA (0.89, 0.90, 0.27, 1.0);
    Colour offWhite = Colour::fromFloatRGBA (0.83, 0.83, 0.89, 1.0);
    Colour grey = Colour::fromFloatRGBA (0.42, 0.42, 0.42, 1.0);
    Colour blackGrey = Colour::fromFloatRGBA (0.2, 0.2, 0.2, 1.0);
};
