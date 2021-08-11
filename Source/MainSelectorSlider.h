/*
  ==============================================================================

    MainSelectorSlider.h
    Created: 31 Jul 2021 1:53:10pm
    Author:  Héctor Ullate Catalán

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LookAndFeelMainSelectorSlider.h"

class MainSelectorSlider : public Slider {

public:
    MainSelectorSlider();
    ~MainSelectorSlider() override;
    
    void mouseDrag(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;
        
private:
    LookAndFeelMainSelectorSlider customLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainSelectorSlider)
};
