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
#include "TwoValueSliderAttachment.h"
#include "LookAndFeelPowerButton.h"
#include "MainSelectorSlider.h"


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
    void setUpUI();
    ToggleButton auxOnOffButton;
        
    CustomRotarySlider gainSlider, mixSlider, smoothSlider;
    MainSelectorSlider mainSelectorSlider;
    
    const int rotarySliderWidth = 80;
    const int separatorTop = 55;
    
    CustomTwoValueSlider filterSlider;
    TwoValueSliderAttachment twoValueSliderAttachment;
    
    LookAndFeelPowerButton lookAndFeelPowerButton;
    
    const double timeDivisionArray[8] = { 0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8 };
  
    CustomRotarySlider tabMixSlider, mainPanSlider, auxPanSlider;
    TextButton mainHalftimeButton {"MAIN TAB"}, auxHalftimeButton {"AUX TAB"};
    
    //Esta variable va a almacenar la TAB seleccionada (Main o Aux), de manera que podamos cambiar el intervalo
    //en ambas iteraciones de halftime. Por defecto, estara seleccionada la Tab Main
    int currentSelectedIndex = 0;
    int selectedSelectorPosition[2] = { 4, 4 };
};
