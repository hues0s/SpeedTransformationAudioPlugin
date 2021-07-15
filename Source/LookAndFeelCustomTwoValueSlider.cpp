/*
  ==============================================================================

    LookAndFeelCustomTwoValueSlider.cpp
    Created: 13 Jul 2021 1:24:58pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelCustomTwoValueSlider.h"


void LookAndFeelCustomTwoValueSlider::drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) {
    
    Point<float> startPoint ((float) x, (float) y + (float) height * 0.5f);
    Point<float> endPoint ((float) (width + x), startPoint.y);

    //Línea punteada de fondo
    Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(blackGrey);
    PathStrokeType pathStrokeType (1.0);
    float dashedLength[2] = {2, 4};
    pathStrokeType.createDashedStroke(backgroundTrack, backgroundTrack, dashedLength, 2);
    g.strokePath(backgroundTrack, pathStrokeType);

    //Línea principal
    Path valueTrack;
    Point<float> minPoint, maxPoint;
    minPoint = { minSliderPos, (float) height * 0.5f };
    maxPoint = { maxSliderPos, (float) height * 0.5f };
                
    valueTrack.startNewSubPath (minPoint);
    valueTrack.lineTo (maxPoint);
    g.setColour(Colours::lightcoral);
    g.strokePath(valueTrack, { 3.5f, PathStrokeType::curved, PathStrokeType::rounded });
            
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
        
        g.setColour(Colours::white);
        g.drawRoundedRectangle(Rectangle<float>(static_cast<float> (maxThumbWidth), static_cast<float>(maxThumbWidth)).withCentre (maxPoint), 50, 1.0);
        g.drawRoundedRectangle(Rectangle<float> (static_cast<float> (minThumbWidth), static_cast<float> (minThumbWidth)).withCentre (minPoint), 50, 1.0);
    }
    
    //Valor del slider
    auto bounds = Rectangle<float>(x, y, width, height);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    g.setColour(Colours::white);
    g.setFont(radius * 0.3f);
    g.drawText(String(slider.getMinValue()), x , y + 22, width, height, Justification::centredLeft);
    g.drawText(String(slider.getMaxValue()), x, y + 22, width, height, Justification::centredRight);

    
    //USAR ESTO DE ABAJO PARA CREAR UN CUSTOM BUTTON
    
    auto length = 3;
    auto thick  = 2.5;
    
    g.setColour (findColour (juce::Slider::textBoxTextColourId));
    //          fromX       fromY        toX                  toY
    g.drawLine (0,          0,           0,                   length,               thick);
    g.drawLine (0,          0,           length,              0,                    thick);
    g.drawLine (0,          height, 0,                   height - length, thick);
    g.drawLine (0,          height, length,              height,          thick);
    g.drawLine (width, height, width - length, height,          thick);
    g.drawLine (width, height, width,          height - length, thick);
    g.drawLine (width, 0,           width - length, 0,                    thick);
    g.drawLine (width, 0,           width,          length,               thick);
    
}
