/*
 =====================================================================================

    Este fichero contiene código que interviene en la creación de la GUI del plugin.

 =====================================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomRotarySlider.h"
#include "CustomTwoValueSlider.h"


class TFGAudioProcessorEditor : public AudioProcessorEditor,  public Slider::Listener {
    
public:
    TFGAudioProcessorEditor (TFGAudioProcessor&);
    ~TFGAudioProcessorEditor() override;

    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void sliderValueChanged(Slider* slider) override;
    
private:
    //Esta referencia esta creada con el objetivo de que el editor pueda acceder al objeto processor que lo ha creado.
    TFGAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TFGAudioProcessorEditor)
    
    //==============================================================================
    Label bpmLabel;
    void setUpUI();
    TextButton
        button1_16 { "1/16" },
        button1_8T { "1/8 T" },
        button1_8 { "1/8" },
        button1_4 { "1/4" },
        button1_2 { "1/2" },
        button1Bar { "1 Bar" },
        button2Bars { "2 Bars" },
        button4Bars { "4 Bars" },
        button8Bars { "8 Bars" };
    TextButton pluginOnOffButton { "On / Off" };
    
    void setUpBarButton(TextButton & button, int radioGroupId);
    
    const int barButtonWidth = 70;
    const int barButtonHeight = 30;
    const int barButtonYFactor = 100;
        
    CustomRotarySlider gainSlider, mixSlider, smoothSlider;
    
    const int rotarySliderWidth = 80;
    const int rotarySliderHeigth = 120;
    const int filterSliderWidth = 30;
    
    const int effectButtonHeigths[3] = { 90, 160, 230 };

    CustomTwoValueSlider filterSlider;
};
