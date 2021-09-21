/*
  ==============================================================================

    LookAndFeelCustomTwoValueSlider.cpp
    Created: 13 Jul 2021 1:24:58pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelCustomTwoValueSlider.h"
#include "ProjectColours.h"


Slider::SliderLayout LookAndFeelCustomTwoValueSlider::getSliderLayout(Slider& slider) {
    Slider::SliderLayout layout;
    layout.sliderBounds = Rectangle<int>(0, 82 + 25, slider.getWidth(), slider.getHeight() - 140 - 30 - 5);
    return layout;
}

void LookAndFeelCustomTwoValueSlider::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) {
    
    auto bounds = Rectangle<float>(x, y - 25, width, height + 50);
    
    Point<float> startPoint ((float) width/2, (float) bounds.getY());
    Point<float> endPoint (startPoint.x, (float) bounds.getHeight() + bounds.getY());

    //Puntos movibles
    Path valueTrack;
    Point<float> minPoint, maxPoint;
    minPoint = { (float) width/2, minSliderPos };
    maxPoint = { minPoint.x, maxSliderPos };
    
    g.setColour(Colours::black);
    g.fillRoundedRectangle(bounds.getX() + 10, bounds.getY() - 25, bounds.getWidth() - 20, bounds.getHeight() + 50, 10);
    
    //Linea de color (se mueve junto a los dos puntos movibles)
    Path backgroundColorTrack;
    backgroundColorTrack.startNewSubPath(startPoint);
    backgroundColorTrack.lineTo(maxPoint);
    g.setColour(ProjectColours::mainColour);
    g.strokePath(backgroundColorTrack, { 55, PathStrokeType::mitered, PathStrokeType::butt });
    
    backgroundColorTrack.clear();
    backgroundColorTrack.startNewSubPath(endPoint);
    backgroundColorTrack.lineTo(minPoint);
    g.setColour(ProjectColours::mainColour);
    g.strokePath(backgroundColorTrack, { 55, PathStrokeType::mitered, PathStrokeType::butt });
    
    //Línea de fondo
    Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(Colour::fromRGBA(40, 40, 40, 255));
    g.strokePath(backgroundTrack, { 50, PathStrokeType::mitered, PathStrokeType::butt });
    
    // TwoValueHorizontal SliderThumb
    g.setColour(Colours::white);
    g.fillRoundedRectangle(maxPoint.getX() - 20, maxPoint.getY() - 15, 40, 15, 5);
    g.fillRoundedRectangle(minPoint.getX() - 20, minPoint.getY(), 40, 15, 5);
    
    //Valor del slider
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::PluginFont_otf, BinaryData::PluginFont_otfSize));
    g.setColour(Colours::white);
    f.setHeight(12);
    g.setFont(f);
    g.drawText(String(slider.getMinValue()) + " Hz", x , bounds.getHeight() + 90, width, 10, Justification::centred);
    g.drawText(String(slider.getMaxValue()) + " Hz", x, bounds.getY() - 17, width, 10, Justification::centred);
    
    //Nombre del slider
    f.setHeight(17);
    g.setFont(f);
    g.drawText(slider.getName(), bounds.getCentreX() - bounds.getWidth()/2, bounds.getY() - 55, bounds.getWidth(), 20, Justification::centredTop);
}
