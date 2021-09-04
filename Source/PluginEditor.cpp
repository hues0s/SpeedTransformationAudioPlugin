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
    //g.setGradientFill(ColourGradient(Colour::fromRGBA(80, 80, 80, 255), getWidth() / 2, 0, Colour::fromRGBA(40, 40, 40, 255), getWidth() / 2, 200, false));
    g.fillAll(ProjectColours::generalBackground);
    
    //Creación de las líneas separadoras
    g.setColour(Colour::fromRGBA(37,38,42,255));
    g.fillRect(Rectangle<int>(0,0, getWidth(), separatorTop));
    
    //g.setGradientFill(ColourGradient(Colour::fromRGBA(60,60,60,255), (getWidth()-150)/2, separatorTop, Colour::fromRGBA(40,40,40,255), (getWidth()-150)/2, getHeight(), false));
    g.fillRect(Rectangle<int>(getWidth() - 150, separatorTop, getWidth(), getHeight()));
    
    Path line;
    g.setColour(Colours::black); //Crear constante global "color de linea"
    
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
    
    //Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    Font f = Font(Typeface::createSystemTypefaceFor(BinaryData::CORPOREA_TTF, BinaryData::CORPOREA_TTFSize));
    f.setHeight(34);
    g.setFont(f);
    g.setColour(Colour::fromRGBA(241,241,237,255));
    g.drawText("SNAIL IT", 70, 2, 300, separatorTop, Justification::centredLeft);
    
    Image mainIcon = ImageCache::getFromMemory(BinaryData::mainIcon_png, BinaryData::mainIcon_pngSize);
    g.drawImage(mainIcon, Rectangle<float>(17, separatorTop/2 - 20, 40, 40), true);
    
    Image ucmIcon = ImageCache::getFromMemory(BinaryData::ucmIcon_png, BinaryData::ucmIcon_pngSize);
    g.drawImage(ucmIcon, Rectangle<float>(getWidth() - 115, separatorTop/2 - 16.5, 70.34, 33), true);
    
    Font f2 = Font(Typeface::createSystemTypefaceFor(BinaryData::TambaSansBold_otf, BinaryData::TambaSansBold_otfSize));
    f2.setHeight(10);
    g.setFont(f2);
    g.setColour(Colour::fromRGBA(241,241,237,255));
    g.drawFittedText("UNIVERSIDAD\nCOMPLUTENSE", getWidth() - 95, 0, 100, separatorTop, Justification::centred, 2);
    
}

void TFGAudioProcessorEditor::resized() {
         
    pluginOnOffButton.setBounds(10,10,40,40);//getWidth() / 2 - 40, 20, 80, 80);
    
    filterSlider.setBounds(getWidth() - 150*2*3/4 - 100/2, separatorTop, 100, getHeight() - separatorTop);
    
    smoothSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 0 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    mixSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 1 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    gainSlider.setBounds(getWidth() - 150/2 - rotarySliderWidth/2, (getHeight() - separatorTop)/3 * 2 + separatorTop, rotarySliderWidth, (getHeight() - separatorTop)/3);
    
    mainSelectorSlider.setBounds(Rectangle<int>(getWidth() - 475, getWidth() - 475).withCentre(Point<int>((getWidth() - 300)/2,(getHeight() - separatorTop)/2 + separatorTop - 25)));
    
    halftimeMixSlider.setBounds(Rectangle<int>(getWidth() - 605, getWidth() - 605).withCentre(Point<int>((getWidth() - 300)/2, getHeight() - 60)));
    mainHaltimePanSlider.setBounds(Rectangle<int>(getWidth() - 605, getWidth() - 605).withCentre(Point<int>((getWidth() - 300)/2 + 90, getHeight() - 60)));
    extraHalftimePanSlider.setBounds(Rectangle<int>(getWidth() - 605, getWidth() - 605).withCentre(Point<int>((getWidth() - 300)/2 - 90, getHeight() - 60)));
    
}

//==============================================================================

void TFGAudioProcessorEditor::sliderValueChanged(Slider *slider) {
    
    if (slider == &gainSlider) audioProcessor.currentDecibels = gainSlider.getValue();
    
    else if (slider == &mainSelectorSlider) audioProcessor.mainSelectorListener(timeDivisionArray[int(mainSelectorSlider.getValue())]);
    
    else if (slider == &mixSlider) audioProcessor.currentDryWetMix = int(mixSlider.getValue());
    
}

void TFGAudioProcessorEditor::setUpUI() {
    
    setSize(675, 470);
    setResizable(false, false);

    mainSelectorSlider.addListener(this);
    addAndMakeVisible(mainSelectorSlider);
    
    //Creamos el gain slider y configuramos su aspecto visual.
    gainSlider.init(-60, 0, 0.1, "Gain", "db");
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider); //Añadimos el componente gainSlider a este componente principal, y lo hacemos visible.
    
    mixSlider.init(0.0, 100.0, 1.0, "Mix", "%");
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    smoothSlider.init(0.0, 100.0, 0.1, "Smooth", "ms");
    smoothSlider.addListener(this);
    addAndMakeVisible(smoothSlider);
    
    
    //Creamos el filter slider (high & low pass) y configuramos su aspecto visual.
    filterSlider.init(20.0, 20000.0, 1.0, "Filter");
    filterSlider.addListener(this);
    addAndMakeVisible(filterSlider);
    
    
    pluginOnOffButton.setToggleState(true, dontSendNotification);
    pluginOnOffButton.onClick = [this] { audioProcessor.pluginOnOffButtonCallback(pluginOnOffButton.getToggleStateValue().getValue()); };
    pluginOnOffButton.setLookAndFeel(&lookAndFeelPowerButton);
    //addAndMakeVisible(pluginOnOffButton);
    
    //Halftime controls
    addAndMakeVisible(halftimeMixSlider);
    addAndMakeVisible(mainHaltimePanSlider);
    addAndMakeVisible(extraHalftimePanSlider);
    
}
