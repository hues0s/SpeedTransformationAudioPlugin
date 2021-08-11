/*
  ==============================================================================

    LookAndFeelCustomTwoValueSlider.cpp
    Created: 13 Jul 2021 1:24:58pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelCustomTwoValueSlider.h"
#include "ProjectColours.h"

/*
void LookAndFeelCustomTwoValueSlider::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) {
    
    auto textBounds = Rectangle<float>(x, y, width, height).reduced (5.0f);
    auto sliderBorderBounds = Rectangle<float>(textBounds.getX() + 10, textBounds.getY() + 25, textBounds.getWidth() - 20, textBounds.getHeight() - 25);
    auto bounds = Rectangle<float>(sliderBorderBounds.getX(), sliderBorderBounds.getY() + 30, sliderBorderBounds.getWidth(), sliderBorderBounds.getHeight() - 60);
    
    
    
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    Point<float> startPoint ((float) width/2, (float) bounds.getY());
    Point<float> endPoint (startPoint.x, (float) bounds.getHeight() + bounds.getY());

    //Puntos movibles
    Path valueTrack;
    Point<float> minPoint, maxPoint;
    minPoint = { (float) width/2, minSliderPos };
    
    maxPoint = { minPoint.x, maxSliderPos };
    
    g.setColour(Colours::black);
    g.fillRoundedRectangle(sliderBorderBounds, 10);
    
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
    auto minThumbWidth = getSliderThumbRadius (slider);
    auto maxThumbWidth = getSliderThumbRadius (slider);

    int thumbBeingDragged = slider.getThumbBeingDragged();
            
    if (thumbBeingDragged == 1 || thumbBeingDragged == 2) {
        slider.setMouseCursor(MouseCursor::NoCursor);
        
        // Thumb being dragged
        g.setColour(Colours::grey);
        g.fillEllipse(Rectangle<float> (static_cast<float> (thumbBeingDragged == 1 ? minThumbWidth : maxThumbWidth), static_cast<float> (thumbBeingDragged == 1 ? minThumbWidth : maxThumbWidth)).withCentre (thumbBeingDragged == 1 ? minPoint : maxPoint));
        
        // The other Thumb not dragged
        g.setColour(Colours::white);
        g.drawRoundedRectangle(Rectangle<float> (static_cast<float> (thumbBeingDragged == 1 ? maxThumbWidth : minThumbWidth), static_cast<float> (thumbBeingDragged == 1 ? maxThumbWidth : minThumbWidth)).withCentre (thumbBeingDragged == 1 ? maxPoint : minPoint), 50, 1.0);
    }
    else {
        slider.setMouseCursor (MouseCursor::NormalCursor);
        
        //Rectangulo 1
        
        Point<float> centreRectangleTop = maxPoint;
        centreRectangleTop.setY(centreRectangleTop.getY() + 20);
        Rectangle<float> rectTop = Rectangle<float>(22.0, 55.0).withCentre(centreRectangleTop);
        
        g.setGradientFill(ColourGradient(Colour::fromRGBA(95, 95, 95, 255), rectTop.getCentreX(), rectTop.getY(), Colour::fromRGBA(65, 65, 65, 255), rectTop.getCentreX(), rectTop.getY() + rectTop.getHeight(), false));
        //g.fillRoundedRectangle(rectTop, 3);
        g.setColour(Colours::black);
        //g.drawRoundedRectangle(rectTop, 3, 1.0f);
        
        
        //Linea central 1
        Path centerLineTop;
        
        centerLineTop.startNewSubPath(Point<float>(rectTop.getWidth() + 18, rectTop.getCentreY() - 1));
        centerLineTop.lineTo(Point<float>(rectTop.getX() + 21, rectTop.getCentreY() - 1));
        g.setColour(Colours::black);
        //g.strokePath(centerLineTop, { 4.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineTop.clear();
        
        centerLineTop.startNewSubPath(Point<float>(rectTop.getWidth() + 18, rectTop.getCentreY() + 0.5));
        centerLineTop.lineTo(Point<float>(rectTop.getX() + 21, rectTop.getCentreY() + 0.5));
        g.setColour(Colours::black);
        //g.strokePath(centerLineTop, { 4.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineTop.clear();
        
        centerLineTop.startNewSubPath(Point<float>(rectTop.getWidth() + 18, rectTop.getCentreY()));
        centerLineTop.lineTo(Point<float>(rectTop.getX() + 21, rectTop.getCentreY()));
        g.setColour(ProjectColours::mainColour);
        //g.strokePath(centerLineTop, { 3.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineTop.clear();
        
        //Rectangulo 2
        
        Point<float> centreRectangleBottom = minPoint;
        centreRectangleBottom.setY(centreRectangleBottom.getY() - 20);
        Rectangle<float> rectBottom = Rectangle<float>(22.0, 55.0).withCentre(centreRectangleBottom);
        
        g.setGradientFill(ColourGradient(Colour::fromRGBA(95, 95, 95, 255), rectBottom.getCentreX(), rectBottom.getY(), Colour::fromRGBA(65, 65, 65, 255), rectBottom.getCentreX(), rectBottom.getY() + rectBottom.getHeight(), false));
        //g.fillRoundedRectangle(rectBottom, 3);
        g.setColour(Colours::black);
        //g.drawRoundedRectangle(rectBottom, 3, 1.0f);
        
        
        //Linea central 2
        Path centerLineBottom;
        
        centerLineBottom.startNewSubPath(Point<float>(rectBottom.getWidth() + 18, rectBottom.getCentreY() - 1));
        centerLineBottom.lineTo(Point<float>(rectBottom.getX() + 21, rectBottom.getCentreY() - 1));
        g.setColour(Colours::black);
        //g.strokePath(centerLineBottom, { 4.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineBottom.clear();
        
        centerLineBottom.startNewSubPath(Point<float>(rectBottom.getWidth() + 18, rectBottom.getCentreY() + 0.5));
        centerLineBottom.lineTo(Point<float>(rectBottom.getX() + 21, rectBottom.getCentreY() + 0.5));
        g.setColour(Colours::black);
        //g.strokePath(centerLineBottom, { 4.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineTop.clear();
        
        centerLineBottom.startNewSubPath(Point<float>(rectBottom.getWidth() + 18, rectBottom.getCentreY()));
        centerLineBottom.lineTo(Point<float>(rectBottom.getX() + 21, rectBottom.getCentreY()));
        g.setColour(ProjectColours::mainColour);
        //g.strokePath(centerLineBottom, { 3.0f, PathStrokeType::mitered, PathStrokeType::square });
        centerLineBottom.clear();
        
    }
    
    //Valor del slider
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    g.setColour(Colours::white);
    f.setHeight(13);
    g.setFont(f);
    g.drawText(String(slider.getMinValue()) + " Hz", x , sliderBorderBounds.getHeight() + 21, width, 10, Justification::centred);
    g.drawText(String(slider.getMaxValue()) + " Hz", x, sliderBorderBounds.getY() + 11, width, 10, Justification::centred);
    
    
    //Nombre del slider
    f.setHeight(17);
    g.setFont(f);
    g.drawText(slider.getName(), textBounds.getCentreX() - textBounds.getWidth()/2, textBounds.getY() - 2, textBounds.getWidth(), 20, Justification::centredTop);
    
} */

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
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
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
