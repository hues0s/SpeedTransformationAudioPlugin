/*
 =====================================================================================

    Este fichero contiene código que interviene en la creación de la GUI del plugin.

 =====================================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProjectColours.h"

TFGAudioProcessorEditor::TFGAudioProcessorEditor(TFGAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), twoValueSliderAttachment(audioProcessor.apvts, "Low Cut Frequency", "High Cut Frequency", filterSlider) {
        setUpUI();
}

TFGAudioProcessorEditor::~TFGAudioProcessorEditor() { }

void TFGAudioProcessorEditor::paint (Graphics& g) {
        
    //Gradientes de fondo
    g.setGradientFill(ColourGradient(Colour::fromRGB(90, 90, 90), 0, separatorTop, Colour::fromRGB(60,60,60), 0, getHeight(), false));
    g.fillRect(Rectangle<int>(0, separatorTop, getWidth() - 150 * 2, getHeight()));
    
    g.setGradientFill(ColourGradient(Colour::fromRGB(65, 65, 65), 0, separatorTop, Colour::fromRGB(45, 45, 45), 0, getHeight(), false));
    g.fillRect(Rectangle<int>(getWidth() - 150 * 2, separatorTop, 150, getHeight()));
    
    g.setGradientFill(ColourGradient(Colour::fromRGB(50, 50, 50), 0, separatorTop, Colour::fromRGB(25, 25, 25), 0, getHeight(), false));
    g.fillRect(Rectangle<int>(getWidth() - 150, separatorTop, 150, getHeight()));
    
    //Creación de las líneas separadoras
    g.setColour(Colour::fromRGBA(37,38,42,255));
    g.fillRect(Rectangle<int>(0,0, getWidth(), separatorTop));
    
    Path line;
    g.setColour(Colours::black);
    
    line.addLineSegment(Line<float>(0, separatorTop, getWidth(), separatorTop), 0.5f);
    g.strokePath(line, PathStrokeType (0.8f, PathStrokeType::mitered, PathStrokeType::butt));
    line.clear();
    
    line.addLineSegment(Line<float>(getWidth() - 150, separatorTop, getWidth() - 150, getHeight()), 0.5f);
    g.strokePath(line, PathStrokeType (0.8f, PathStrokeType::mitered, PathStrokeType::butt));
    line.clear();
    
    line.addLineSegment(Line<float>(getWidth() - 150, (getHeight() - separatorTop)/3 * 1 + separatorTop, getWidth(), (getHeight() - separatorTop)/3 * 1 + separatorTop), 0.5f);
    g.strokePath(line, PathStrokeType (0.8f, PathStrokeType::mitered, PathStrokeType::butt));
    line.clear();
    
    line.addLineSegment(Line<float>(getWidth() - 150, (getHeight() - separatorTop)/3 * 2 + separatorTop, getWidth(), (getHeight() - separatorTop)/3 * 2 + separatorTop), 0.5f);
    g.strokePath(line, PathStrokeType (0.8f, PathStrokeType::mitered, PathStrokeType::butt));
    line.clear();
    
    line.addLineSegment(Line<float>(getWidth() - 150*2, separatorTop, getWidth() - 150*2, getHeight()), 0.5f);
    g.strokePath(line, PathStrokeType (0.8f, PathStrokeType::mitered, PathStrokeType::butt));
    line.clear();
    
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    f.setHeight(38);
    g.setFont(f);
    g.setColour(Colour::fromRGBA(241,241,237,255));
    g.drawText("TFG", 20, 0, 300, separatorTop, Justification::centredLeft);
    
    Image ucmIcon = ImageCache::getFromMemory(BinaryData::ucmIcon_png, BinaryData::ucmIcon_pngSize);
    g.drawImage(ucmIcon, Rectangle<float>(getWidth() - 115, separatorTop/2 - 16.5, 70.34, 33), true);
    
    Font f2 = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    f2.setHeight(10);
    g.setFont(f2);
    g.setColour(Colour::fromRGBA(241,241,237,255));
    g.drawFittedText("UNIVERSIDAD\nCOMPLUTENSE", getWidth() - 95, 0, 100, separatorTop, Justification::centred, 2);
    
    g.setColour(Colours::black);
    g.fillRoundedRectangle(35 - 2, getHeight() - 135 - 2, getWidth() - 150 * 2 - 35 * 2 - 2 + 4, 125 + 4, 22);
    g.setColour(Colour::fromRGB(115, 115, 115));
    g.fillRoundedRectangle(35, getHeight() - 135, getWidth() - 150 * 2 - 35 * 2 - 2, 125, 20);
}

void TFGAudioProcessorEditor::resized() {
    
    filterSlider.setBounds(getWidth() - 150*2*3/4 - 100/2, separatorTop, 100, getHeight() - separatorTop);
    
    smoothSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 0 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    mixSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 1 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    gainSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 2 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    
    Rectangle<int> mainRectangle = Rectangle<int>(getWidth() - 475, getWidth() - 475).withCentre(Point<int>((getWidth() - 300)/2,(getHeight() - separatorTop)/2 + separatorTop - 25));
    
    mainSelectorSlider.setBounds(mainRectangle.getX(), mainRectangle.getY() - 20, mainRectangle.getWidth(), mainRectangle.getHeight());
    
    tabMixSlider.setBounds((getWidth() - 300)/2 - rotarySliderWidth/2, mainRectangle.getY() + 200 - 5, rotarySliderWidth - 6, (getHeight() - separatorTop)/3 - 6);
    mainPanSlider.setBounds((getWidth() - 300)/2 - rotarySliderWidth/2 - 100, mainRectangle.getY() + 200 - 5, rotarySliderWidth - 6, (getHeight() - separatorTop)/3 - 6);
    auxPanSlider.setBounds((getWidth() - 300)/2 - rotarySliderWidth/2 + 100, mainRectangle.getY() + 200 - 5, rotarySliderWidth - 6, (getHeight() - separatorTop)/3 - 6);
    
    mainHalftimeButton.setBounds(mainRectangle.getCentreX() - 135 - 9, mainRectangle.getY() - 65, 120, 30);
    auxOnOffButton.setBounds(mainRectangle.getCentreX() + 2, 85 - 5, 15, 15);
    auxHalftimeButton.setBounds(mainRectangle.getCentreX() + 35 - 9, mainRectangle.getY() - 65, 120, 30);
}

//==============================================================================

void TFGAudioProcessorEditor::sliderValueChanged(Slider *slider) {
    
    if (slider == &gainSlider) audioProcessor.currentDecibels = gainSlider.getValue();
    else if (slider == &mainSelectorSlider) {
        selectedSelectorPosition[currentSelectedIndex] = mainSelectorSlider.getValue();
        audioProcessor.selectorListener(timeDivisionArray[int(selectedSelectorPosition[currentSelectedIndex])], currentSelectedIndex);
    }
    else if (slider == &mixSlider) audioProcessor.currentDryWetMix = int(mixSlider.getValue());
    else if (slider == &smoothSlider) audioProcessor.hasToSmooth = bool(smoothSlider.getValue());
    else if (slider == &mainPanSlider) audioProcessor.currentMainPan = mainPanSlider.getValue();
    else if (slider == &auxPanSlider) audioProcessor.currentAuxPan = auxPanSlider.getValue();
    else if (slider == &tabMixSlider) audioProcessor.currentDryWetTabMix = int(tabMixSlider.getValue());
}

void TFGAudioProcessorEditor::setUpUI() {
    
    setSize(675, 470);
    setResizable(false, false);

    mainSelectorSlider.addListener(this);
    addAndMakeVisible(mainSelectorSlider);
    
    //Creamos el gain slider y configuramos su aspecto visual.
    gainSlider.init(-60, 0, 0.1, "Gain", "dB");
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider); //Añadimos el componente gainSlider a este componente principal, y lo hacemos visible.
    
    mixSlider.init(0.0, 100.0, 1.0, "Mix", "%");
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    smoothSlider.init(0.0f, 1.0f, 1.0f, "Smooth", "ON/OFF");
    smoothSlider.addListener(this);
    addAndMakeVisible(smoothSlider);
    
    
    //Creamos el filter slider (high & low pass) y configuramos su aspecto visual.
    filterSlider.init(20.0, 20000.0, 1.0, "Filter");
    filterSlider.addListener(this);
    addAndMakeVisible(filterSlider);
    
    
    auxOnOffButton.setToggleState(true, dontSendNotification);
    auxOnOffButton.onClick = [this] { audioProcessor.auxOnOffButtonCallback(auxOnOffButton.getToggleStateValue().getValue()); };
    auxOnOffButton.setLookAndFeel(&lookAndFeelPowerButton);
    addAndMakeVisible(auxOnOffButton);
    
    //Halftime controls
    tabMixSlider.init(0.0, 100.0, 1.0, "Tab Mix", "%");
    tabMixSlider.addListener(this);
    addAndMakeVisible(tabMixSlider);
    
    mainPanSlider.init(-1.0, 1.0, 0.1, "Main Pan", "");
    mainPanSlider.setValue(0.0);
    mainPanSlider.addListener(this);
    addAndMakeVisible(mainPanSlider);
    
    auxPanSlider.init(-1.0, 1.0, 0.1, "Aux Pan", "");
    auxPanSlider.setValue(0.0);
    auxPanSlider.addListener(this);
    addAndMakeVisible(auxPanSlider);
    
    mainHalftimeButton.setRadioGroupId(1);
    mainHalftimeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    mainHalftimeButton.setToggleState(true, dontSendNotification);
    mainHalftimeButton.setClickingTogglesState(true);
    mainHalftimeButton.onClick = [this] {
        currentSelectedIndex = 0;
        //Cada vez que cambiamos de tab, debemos recuperar la posicion previa que tenia esa TAB
        mainSelectorSlider.setValue(selectedSelectorPosition[currentSelectedIndex]);
    };
    addAndMakeVisible(mainHalftimeButton);
    
    auxHalftimeButton.setRadioGroupId(1);
    auxHalftimeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    auxHalftimeButton.setClickingTogglesState(true);
    auxHalftimeButton.onClick = [this] {
        currentSelectedIndex = 1;
        mainSelectorSlider.setValue(selectedSelectorPosition[currentSelectedIndex]);
    };
    addAndMakeVisible(auxHalftimeButton);
    
}
