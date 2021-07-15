/*
 =====================================================================================

    Este fichero contiene código que interviene en la creación de la GUI del plugin.

 =====================================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"


TFGAudioProcessorEditor::TFGAudioProcessorEditor (TFGAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p) {
        setUpUI();
}

TFGAudioProcessorEditor::~TFGAudioProcessorEditor() { }

void TFGAudioProcessorEditor::paint (Graphics& g) {
    g.fillAll(Colours::black); //Pintamos el fondo del plugin de color negro.
}

void TFGAudioProcessorEditor::resized() {
         
    pluginOnOffButton.setBounds(getWidth() / 2 - 40, 20, 80, 30);
    
    button1_16.setBounds(getWidth() / 2 - barButtonWidth / 2 - 90, effectButtonHeigths[0], barButtonWidth, barButtonHeight);
    button1_8T.setBounds(getWidth() / 2 - barButtonWidth / 2, effectButtonHeigths[0], barButtonWidth, barButtonHeight);
    button1_8.setBounds(getWidth() / 2 - barButtonWidth / 2 + 90, effectButtonHeigths[0], barButtonWidth, barButtonHeight);
    button1_4.setBounds(getWidth() / 2 - barButtonWidth / 2 - 90, effectButtonHeigths[1], barButtonWidth, barButtonHeight);
    button1_2.setBounds(getWidth() / 2 - barButtonWidth / 2, effectButtonHeigths[1], barButtonWidth, barButtonHeight);
    button1Bar.setBounds(getWidth() / 2 - barButtonWidth / 2 + 90, effectButtonHeigths[1], barButtonWidth, barButtonHeight);
    button2Bars.setBounds(getWidth() / 2 - barButtonWidth / 2 - 90, effectButtonHeigths[2], barButtonWidth, barButtonHeight);
    button4Bars.setBounds(getWidth() / 2 - barButtonWidth / 2, effectButtonHeigths[2], barButtonWidth, barButtonHeight);
    button8Bars.setBounds(getWidth() / 2 - barButtonWidth / 2 + 90, effectButtonHeigths[2], barButtonWidth, barButtonHeight);
    
    filterSlider.setBounds(filterSliderWidth, getHeight() - 230, getWidth() - filterSliderWidth * 2, 100);
    
    smoothSlider.setBounds(getWidth()/2 - rotarySliderWidth/2 - 100, getHeight() - 150, rotarySliderWidth, rotarySliderHeigth);
    mixSlider.setBounds(getWidth()/2 - rotarySliderWidth/2, getHeight() - 150, rotarySliderWidth, rotarySliderHeigth);
    gainSlider.setBounds(getWidth()/2 - rotarySliderWidth/2 + 100, getHeight() - 150, rotarySliderWidth, rotarySliderHeigth);

}

//==============================================================================

void TFGAudioProcessorEditor::sliderValueChanged(Slider *slider) {
    
    if (slider == &gainSlider) {
        audioProcessor.currentDecibels = gainSlider.getValue();
    }
    
}

void TFGAudioProcessorEditor::setUpBarButton(TextButton & button, int radioGroupId) {
    button.setToggleState (false, dontSendNotification);
    button.setClickingTogglesState(true);
    button.setRadioGroupId (radioGroupId);
    addAndMakeVisible(button);
}

void TFGAudioProcessorEditor::setUpUI() {
    
    setSize(370, 500);
    
    //Creamos el gain slider y configuramos su aspecto visual.
    gainSlider.init(-60, 0, 0.1, "Gain");
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider); //Añadimos el componente gainSlider a este componente principal, y lo hacemos visible.
    
    mixSlider.init(0.0, 100.0, 1.0, "Mix");
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    smoothSlider.init(0.0, 100.0, 0.1, "Smooth");
    smoothSlider.addListener(this);
    addAndMakeVisible(smoothSlider);
    
    //Creamos el filter slider (high & low pass) y configuramos su aspecto visual.
    filterSlider.init(20.0, 20000.0, 1.0);
    filterSlider.addListener(this);
    addAndMakeVisible(filterSlider);
    
    //Creamos el label de BPM y lo configuramos
    bpmLabel.setText ("test", dontSendNotification);
    bpmLabel.setFont (Font (16.0f, Font::bold));
    bpmLabel.setJustificationType (Justification::topLeft);
    addAndMakeVisible(bpmLabel);
    
    // Configuramos los botones que controlan la duracion del efecto
    setUpBarButton(button1_16, 1001);
    setUpBarButton(button1_8T, 1001);
    setUpBarButton(button1_8, 1001);
    setUpBarButton(button1_4, 1001);
    setUpBarButton(button1_2, 1001);
    button1_2.setToggleState (true, dontSendNotification);
    setUpBarButton(button1Bar, 1001);
    setUpBarButton(button2Bars, 1001);
    setUpBarButton(button4Bars, 1001);
    setUpBarButton(button8Bars, 1001);
    
    button1_16.onStateChange = [this] { if(button1_16.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(0.0625); };
    //button1_8T.onStateChange = [this] { audioProcessor.barButtonChangeCallback(1.0); }; //TODO
    button1_8.onStateChange = [this] { if(button1_8.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(0.125); };
    button1_4.onStateChange = [this] { if(button1_4.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(0.25); };
    button1_2.onStateChange = [this] { if(button1_2.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(0.5); };
    button1Bar.onStateChange = [this] { if(button1Bar.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(1.0); };
    button2Bars.onStateChange = [this] { if(button2Bars.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(2.0); };
    button4Bars.onStateChange = [this] { if(button4Bars.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(4.0); };
    button8Bars.onStateChange = [this] { if(button8Bars.getToggleStateValue() == Button::ButtonState::buttonDown) audioProcessor.barButtonChangeCallback(8.0); };
    
    getLookAndFeel().setColour(TextButton::buttonColourId, Colours::transparentBlack);
    getLookAndFeel().setColour(TextButton::buttonOnColourId, Colours::lightcoral);
    
    pluginOnOffButton.onClick = [this] { audioProcessor.pluginOnOffButtonCallback(); audioProcessor.isPluginOn ? pluginOnOffButton.setColour(TextButton::buttonColourId, Colours::lightcoral) : pluginOnOffButton.setColour(TextButton::buttonColourId, Colours::transparentBlack);  };
    pluginOnOffButton.setColour(TextButton::buttonColourId, Colours::lightcoral);
    addAndMakeVisible(pluginOnOffButton);
    
}
