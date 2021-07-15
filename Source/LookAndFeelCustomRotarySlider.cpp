/*
  ==============================================================================

    LookAndFeelCustomRotarySlider.cpp
    Created: 12 Jul 2021 9:28:27am
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelCustomRotarySlider.h"


void LookAndFeelCustomRotarySlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
    
    auto fillColour = slider.findColour(Slider::rotarySliderFillColourId);
    auto bounds = Rectangle<float>(x, y, width, height).reduced (5.0f);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.1f;
    auto arcRadius = radius - lineW * 0.5f;
    
    Rectangle<float> ellipseArea = Rectangle<float>(bounds.getCentreX() - bounds.getWidth()/2, bounds.getCentreY() - bounds.getWidth()/2, bounds.getWidth(), bounds.getWidth());
    
    //Arco intermedio
    g.setColour(customGrey);
    g.fillEllipse(ellipseArea.reduced (5.0f));
    
    //Arco exterior
    Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, toAngle, true);
    g.setColour(fillColour);
    g.strokePath(valueArc, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::butt));

    //Indicador de posición
    auto thumbWidth = lineW * 2.0f;
    Path thumb;
    thumb.addRectangle(-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius + lineW);
    g.setColour(customWhite);
    g.fillPath(thumb, AffineTransform::rotation (toAngle + 3.12f).translated(bounds.getCentre()));

    //Elipse central
    g.fillEllipse(ellipseArea.reduced(10.0f));
    
    //Valor del slider
    g.setColour(Colours::black);
    g.setFont(radius * 0.4f);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << slider.getValue();
    g.drawText(stream.str(), ellipseArea, Justification::centred);
    
    //Nombre del slider
    g.setColour(Colours::white);
    g.setFont(radius * 0.36f);
    g.drawText(slider.getName(), bounds.getCentreX() - bounds.getWidth()/2, bounds.getWidth() + 5, bounds.getWidth(), bounds.getWidth(), Justification::centred);
}
