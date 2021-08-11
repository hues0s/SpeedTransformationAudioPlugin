/*
  ==============================================================================

    LookAndFeelMainSelectorSlider.cpp
    Created: 31 Jul 2021 1:58:45pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelMainSelectorSlider.h"
#include "ProjectColours.h"


void LookAndFeelMainSelectorSlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
    
    auto fillColour = slider.findColour(Slider::rotarySliderFillColourId);
    auto bounds = Rectangle<float>(x, y + 5, width, height).reduced (5.0f);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.11f;
    auto arcRadius = radius - lineW * 0.65f;
    
    String rotaryValuesStringArray [] = {"1/16", "1/8 T", "1/8", "1/4", "1/2", "1", "2", "4", "8"};
    
    Rectangle<float> ellipseArea = Rectangle<float>(bounds.getCentreX() - bounds.getWidth()/2, bounds.getCentreY() - bounds.getWidth()/2, bounds.getWidth(), bounds.getWidth());
    
    //Divide el arco en 9 posiciones
    float angleInterval = (10.4f - 5.28f)/9 + 0.004f;
    
    g.setColour(Colours::red);
    g.drawRect(ellipseArea);
    
    //Arco exterior (background)
    Path valueArc2;
    //valueArc2.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 130, 130, 0.0f, 3.6f, 8.96f, true);
    valueArc2.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 125, 125, 0.0f, 5.3f, 10.42f, true);
    g.setColour(Colours::black);
    g.strokePath(valueArc2, PathStrokeType (60, PathStrokeType::mitered, PathStrokeType::butt));
    
    Path valueArc3;
    valueArc3.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 125, 125, 0.0f, 5.3f, 10.42f, true);
    g.setColour(Colour::fromRGBA(60, 60, 60, 255));
    g.strokePath(valueArc3, PathStrokeType (45, PathStrokeType::mitered, PathStrokeType::butt));
    
    
    //Selector de posicion
    Path selectedArc;
    g.setColour(Colours::black);
    selectedArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 125, 125, 0.0f, toAngle + 0.13f + slider.getValue() * 0.0037f, toAngle + 0.42f + slider.getValue() * 0.0037f, true);
    g.strokePath(selectedArc, PathStrokeType (80, PathStrokeType::mitered, PathStrokeType::butt));
    
    selectedArc.clear();
    g.setColour(Colours::darkgrey);
    selectedArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 125, 125, 0.0f, toAngle + 0.13f + slider.getValue() * 0.0037f + 0.01f, toAngle + 0.42f + slider.getValue() * 0.0037f - 0.01f, true);
    g.strokePath(selectedArc, PathStrokeType (76.5f, PathStrokeType::mitered, PathStrokeType::butt));
    
    selectedArc.clear();
    selectedArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 125, 125, 0.0f, toAngle + 0.13f + slider.getValue() * 0.0037f + 0.1f, toAngle + 0.42f + slider.getValue() * 0.0037f - 0.1f, true);
    g.setColour(ProjectColours::mainColour);
    g.strokePath(selectedArc, PathStrokeType (60, PathStrokeType::mitered, PathStrokeType::butt));
    
    //Borde superior
    Path thumb;
    g.setColour(Colours::black);
    thumb.addRectangle(0, -154.8f, 7, 59.4f);
    g.fillPath(thumb, AffineTransform::rotation (5.28f).translated(bounds.getCentre()));
    
    //Borde inferior
    thumb.clear();
    g.setColour(Colours::black);
    thumb.addRectangle(0, -155, 7, 60);
    g.fillPath(thumb, AffineTransform::rotation (10.40f).translated(bounds.getCentre()));
    
    //Subdivisiones internas
    thumb.clear();
    thumb.addRectangle(0, -154.8f, 2.5f, 59.4f);
    for(int i = 1; i < 9; ++i){
        g.fillPath(thumb, AffineTransform::rotation (5.28f + angleInterval * i).translated(bounds.getCentre()));
    }
    
    //Texto
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    g.setColour(Colour::fromRGBA(210, 210, 210, 255));
    f.setHeight(60);
    g.setFont(f);
    g.drawText(rotaryValuesStringArray[int(slider.getValue())], bounds.getCentreX() - bounds.getWidth()/2, height/2 - 20, bounds.getWidth(), 20, Justification::centred);
    
    f.setHeight(20);
    g.setFont(f);
    String extraText = slider.getValue() > 5 ? "BARS" : "BAR";
    g.drawText(extraText, bounds.getCentreX() - bounds.getWidth()/2, height/2 + 25, bounds.getWidth(), 20, Justification::centred);
    
    
    
    
    /*
    //Arco exterior
    Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, toAngle - 0.06f, toAngle + 0.2f, true);
    //valueArc.addBubble(bounds, bounds, {5.0, 5.0}, 10.0, 10.0);
    g.setColour(fillColour);
    //g.strokePath(valueArc, PathStrokeType (lineW * 2, PathStrokeType::mitered, PathStrokeType::butt));
    
    
    
    //Borde central
    g.setColour(Colours::black);
    //g.fillEllipse(ellipseArea.reduced(15.0f));
    
    //Elipse central
    g.setGradientFill(ColourGradient(Colour::fromRGBA(150, 150, 150, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() - arcRadius, Colour::fromRGBA(30, 30, 30, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() + arcRadius, false));
    //g.fillEllipse(ellipseArea.reduced(16.5f));
    
    //Borde interior
    g.setColour(Colours::black);
    //g.fillEllipse(ellipseArea.reduced(21.5f));
    
    //Elipse interior
    g.setColour(interiorEllipseColour);
    g.setGradientFill(ColourGradient(Colour::fromRGBA(70, 70, 70, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() - arcRadius, Colour::fromRGBA(105, 105, 105, 255), ellipseArea.reduced(18.0f).getWidth() / 2, bounds.getCentreY() + arcRadius, false));
    //g.fillEllipse(ellipseArea.reduced(23.0f));
    
    //Indicador de posición
    auto thumbWidth = lineW;
    Path thumb;
    g.setColour(Colours::black);
    thumb.addRectangle(-thumbWidth / 2 - 1.5 , -thumbWidth / 2 - 1.5 + 8.0, thumbWidth + 3.0, ellipseArea.reduced(10.0f).getWidth()/2 + 3.0 - 7.8);
    //g.fillPath(thumb, AffineTransform::rotation (toAngle + 3.12f).translated(bounds.getCentre()));

    Path thumb2;
    g.setColour(ProjectColours::mainColour);
    thumb2.addRectangle(-thumbWidth / 2, -thumbWidth / 2 + 8.0, thumbWidth, ellipseArea.reduced(10.0f).getWidth()/2 - 7.8);
    //g.fillPath(thumb2, AffineTransform::rotation (toAngle + 3.12f).translated(bounds.getCentre()));
    
    
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    
    
    //Valor del slider
    g.setColour(Colour::fromRGBA(210, 210, 210, 255));
    f.setHeight(15);
    g.setFont(f);
    
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << slider.getValue() << " " << slider.getComponentID();
    g.drawText(stream.str(), bounds.getCentreX() - bounds.getWidth()/2, height/2 + 45, bounds.getWidth(), 20, Justification::centred);
    
    //Nombre del slider
    g.setColour(Colours::white);
    f.setHeight(20);
    g.setFont(f);
    
    g.drawText(slider.getName(), bounds.getCentreX() - bounds.getWidth()/2, height/2 - 60, bounds.getWidth(), 20, Justification::centred);
        */
}
