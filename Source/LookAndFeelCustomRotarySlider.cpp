/*
  ==============================================================================

    LookAndFeelCustomRotarySlider.cpp
    Created: 12 Jul 2021 9:28:27am
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelCustomRotarySlider.h"
#include "ProjectColours.h"


void LookAndFeelCustomRotarySlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
    
    auto fillColour = slider.findColour(Slider::rotarySliderFillColourId);
    auto bounds = Rectangle<float>(x, y + 5, width, height).reduced (5.0f);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.11f;
    auto arcRadius = radius - lineW * 0.65f;
    
    Rectangle<float> ellipseArea = Rectangle<float>(bounds.getCentreX() - bounds.getWidth()/2 + 2, bounds.getCentreY() - bounds.getWidth()/2 + 2 + 3, bounds.getWidth() - 4, bounds.getWidth() - 4);
    
    //Arco exterior (background)
    Path valueArc2;
    valueArc2.addCentredArc(bounds.getCentreX(), bounds.getCentreY() + 3, arcRadius, arcRadius, 0.0f, 4.1f, 8.5f, true);
    g.setColour(Colours::darkgrey);
    g.strokePath(valueArc2, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::rounded));
    
    if (slider.getMinimum() < 0) {
        auto radian = MathConstants<float>::twoPi - rotaryStartAngle;
        if (slider.getValue() < 0) {
            Path valueArcMinus;
            valueArcMinus.addCentredArc (bounds.getCentreX(), bounds.getCentreY() + 3, arcRadius, arcRadius, 0.0f, 0.0f, jmap (sliderPos, 0.5f, 0.0f, 0.0f, -radian), true);
            g.setColour (fillColour);
            g.strokePath (valueArcMinus, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::rounded));
        }
        else {
            Path valueArcPlus;
            valueArcPlus.addCentredArc (bounds.getCentreX(), bounds.getCentreY() + 3, arcRadius, arcRadius, 0.0f, 0.0f, jmap (sliderPos, 0.5f, 1.0f, 0.0f, radian + 0.02f), true);
            g.setColour (fillColour);
            g.strokePath (valueArcPlus, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::rounded));
        }
    }
    else {
        //Arco exterior
        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY() + 3, arcRadius, arcRadius, 0.0f, rotaryStartAngle, toAngle, true);
        g.setColour(fillColour);
        g.strokePath(valueArc, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::rounded));
    }
    
    
    //Borde central
    g.setColour(Colours::black);
    g.fillEllipse(ellipseArea.reduced(10.0f));
    
    //Elipse central
    g.setGradientFill(ColourGradient(Colour::fromRGBA(150, 150, 150, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() - arcRadius, Colour::fromRGBA(30, 30, 30, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() + arcRadius, false));
    g.fillEllipse(ellipseArea.reduced(11.5f));
    
    //Borde interior
    g.setColour(Colours::black);
    g.fillEllipse(ellipseArea.reduced(16.5f));
    
    //Elipse interior
    g.setColour(interiorEllipseColour);
    g.setGradientFill(ColourGradient(Colour::fromRGBA(70, 70, 70, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() - arcRadius, Colour::fromRGBA(105, 105, 105, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() + arcRadius, false));
    g.fillEllipse(ellipseArea.reduced(18.0f));
    
    //Indicador de posición
    auto thumbWidth = lineW;
    Path thumb;
    g.setColour(Colours::black);
    thumb.addRectangle(-thumbWidth / 2 - 1.5 , -thumbWidth / 2 - 1.5 + 8.0, thumbWidth + 3.0, ellipseArea.reduced(10.0f).getWidth()/2 + 3.0 - 7.8);
    g.fillPath(thumb, AffineTransform::rotation (toAngle + 3.12f).translated(bounds.getCentre()));

    Path thumb2;
    g.setColour(ProjectColours::mainColour);
    thumb2.addRectangle(-thumbWidth / 2, -thumbWidth / 2 + 8.0, thumbWidth, ellipseArea.reduced(10.0f).getWidth()/2 - 7.8);
    g.fillPath(thumb2, AffineTransform::rotation (toAngle + 3.12f).translated(bounds.getCentre()));
    
    
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    
    //Valor del slider
    g.setColour(Colour::fromRGBA(210, 210, 210, 255));
    f.setHeight(15);
    g.setFont(f);
    
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << slider.getValue() << " " << slider.getComponentID();
    g.drawText(stream.str(), bounds.getCentreX() - bounds.getWidth()/2, height/2 + 38, bounds.getWidth(), 20, Justification::centred);
    
    //Nombre del slider
    g.setColour(Colours::white);
    f.setHeight(17);
    g.setFont(f);
    
    g.drawText(slider.getName(), bounds.getCentreX() - bounds.getWidth()/2, height/2 - 55, bounds.getWidth(), 20, Justification::centred);
        
}
