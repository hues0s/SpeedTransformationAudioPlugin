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
    
    auto bounds = Rectangle<float>(x, y + 5, width, height).reduced (5.0f);
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
   
    String rotaryValuesStringArray [] = {"1/16", "1/8", "1/4", "1/2", "1", "2", "4", "8"};
    
    //Divide el arco en 8 posiciones
    float angleInterval = (10.4f - 5.28f)/8 + 0.004f;
    
    //Arco exterior (background)
    Path valueArc2;
    valueArc2.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 70, 70, 0.0f, 5.26f, 10.428f, true);
    g.setColour(Colours::black);
    g.strokePath(valueArc2, PathStrokeType (50, PathStrokeType::mitered, PathStrokeType::butt));
    
    g.setColour(ProjectColours::mainColour);
    g.strokePath(valueArc2, PathStrokeType (39, PathStrokeType::mitered, PathStrokeType::butt));
    
    g.setColour(Colour::fromRGBA(60, 60, 60, 255));
    g.strokePath(valueArc2, PathStrokeType (35, PathStrokeType::mitered, PathStrokeType::butt));
    
    //Selector de posicion
    Path selectedArc;
    g.setColour(Colours::white);
    selectedArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), 70, 70, 0.0f, toAngle + 0.22f - slider.getValue() * 0.0035f, toAngle + 0.38f - slider.getValue() * 0.0035f, true);
    g.strokePath(selectedArc, PathStrokeType (12, PathStrokeType::beveled, PathStrokeType::rounded));
    
    
    //Borde superior
    Path thumb;
    g.setColour(Colours::black);
    thumb.addRectangle(-2.2f, -94.8f, 5.5f, 49.8f);
    g.fillPath(thumb, AffineTransform::rotation (5.28f).translated(bounds.getCentre()));
    
    //Borde inferior
    thumb.clear();
    g.setColour(Colours::black);
    thumb.addRectangle(1.7f, -95.2f, 5.5f, 50);
    g.fillPath(thumb, AffineTransform::rotation (10.40f).translated(bounds.getCentre()));
         
    //Subdivisiones internas
    thumb.clear();
    thumb.addRectangle(0, -86.8f, 1.5f, 33.7f);
    for(int i = 1; i < 8; ++i){
        g.fillPath(thumb, AffineTransform::rotation (5.28f + angleInterval * i).translated(bounds.getCentre()));
    }
    
    //Texto
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    g.setColour(Colour::fromRGBA(210, 210, 210, 255));
    f.setHeight(35);
    g.setFont(f);
    g.drawText(rotaryValuesStringArray[int(slider.getValue())], bounds.getCentreX() - bounds.getWidth()/2, height/2 - 13, bounds.getWidth(), 20, Justification::centred);
    
    f.setHeight(13);
    g.setFont(f);
    String extraText = slider.getValue() > 4 ? "BARS" : "BAR";
    g.drawText(extraText, bounds.getCentreX() - bounds.getWidth()/2, height/2 + 13, bounds.getWidth(), 20, Justification::centred);
    
}
