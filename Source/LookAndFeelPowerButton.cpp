/*
  ==============================================================================

    LookAndFeelPowerButton.cpp
    Created: 25 Jul 2021 11:05:02am
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#include "LookAndFeelPowerButton.h"
#include "ProjectColours.h"


void LookAndFeelPowerButton::drawTickBox (Graphics & g, Component & component, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
    
    float width = 15.0f;
    float height = 15.0f;
    auto buttonArea = Rectangle<float>(0, 0, width, height);
    
    
    //Arco exterior
    Path arc;
    arc.addCentredArc(buttonArea.getCentreX(), buttonArea.getCentreY() + 1, 5.5f, 5.5f, 0.0f, 0.593599f, 5.68959f, true);
    if(ticked){ g.setColour(ProjectColours::mainColour); }
    else { g.setColour(customGrey); }
    g.strokePath(arc, PathStrokeType (2.5f, PathStrokeType::beveled, PathStrokeType::rounded));
    
    //Linea vertical
    Path verticalLine;
    verticalLine.addLineSegment(Line<float>(width/2, 2, width/2, height/2), 1.0f);
    if(ticked){ g.setColour(ProjectColours::mainColour); }
    else { g.setColour(customGrey); }
    g.strokePath(verticalLine, PathStrokeType (1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    
}
