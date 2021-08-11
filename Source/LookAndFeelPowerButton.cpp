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
    
    float width = 40.0f;
    float height = 40.0f;
    auto buttonArea = Rectangle<float>(0, 0, width, height);
    
    
    //Arco exterior
    Path arcBackground;
    arcBackground.addCentredArc(buttonArea.getCentreX(), buttonArea.getCentreY() + 1, 15.0f, 15.0f, 0.0f, 0.523599f, 5.75959f, true);
    g.setColour(Colours::black);
    g.strokePath(arcBackground, PathStrokeType (8.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    Path arc;
    arc.addCentredArc(buttonArea.getCentreX(), buttonArea.getCentreY() + 1, 15.0f, 15.0f, 0.0f, 0.523599f, 5.75959f, true);
    if(ticked){ g.setColour(ProjectColours::mainColour); }
    else { g.setColour(customGrey); }
    g.strokePath(arc, PathStrokeType (6.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    
    //Linea vertical
    Path verticalLineBackground;
    verticalLineBackground.addLineSegment(Line<float>(width/2, 4, width/2, height/2), 1.0f);
    g.setColour(Colours::black);
    g.strokePath(verticalLineBackground, PathStrokeType (8.0f, PathStrokeType::curved, PathStrokeType::rounded));
    Path verticalLine;
    verticalLine.addLineSegment(Line<float>(width/2, 4, width/2, height/2), 1.0f);
    if(ticked){ g.setColour(ProjectColours::mainColour); }
    else { g.setColour(customGrey); }
    g.strokePath(verticalLine, PathStrokeType (6.0f, PathStrokeType::curved, PathStrokeType::rounded));
    
}
