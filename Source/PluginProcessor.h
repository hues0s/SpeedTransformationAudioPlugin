/*
 =====================================================================================

    Este fichero contiene código que gestiona el procesador de audio del plugin.

 =====================================================================================
*/


#pragma once
#include <JuceHeader.h>


class TFGAudioProcessor : public AudioProcessor {

public:
    
    TFGAudioProcessor();
    ~TFGAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    //double selectedTimeDivision = { 1 };
    
    double selectedTimeDivision[2] = { 1, 1 }; //Guardamos las selecciones de Halftime Main [0] y Aux [1] en un mismo array.
    
    float currentDecibels { 0.0 };
    int currentDryWetMix { 100 };
    float currentMainPan { 0.0 };
    float currentAuxPan { 0.0 };
    int currentDryWetTabMix { 100 };
    
    bool isAuxOn = true;
    void auxOnOffButtonCallback(bool isAuxOn);
    
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", createParameterLayout() };
    struct ChainSettings {
        float lowCutFreq { 0 };
        float highCutFreq { 0 };
    };
    
    ChainSettings getChainSettings (AudioProcessorValueTreeState& apvts);
    
    void selectorListener(double changedTimeDivision, int index);
    bool hasToSmooth = true;
    
private:
    
    AudioPlayHead *playHead;
    AudioPlayHead::CurrentPositionInfo cpi;
    int currentNumSamples = 0;
    const double defaultQ = 0.71;
    
    void handleOutputGain(AudioBuffer<float> & buffer, int numChannels, int numSamples);
    void handlePan(AudioBuffer<float> & buffer, int numChannels, int numSamples, float currentPan);
    void handleFilters(AudioBuffer<float> & buffer);
    
    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::IIR::Filter<float>> leftChain, rightChain;
    enum ChainPositions { LowCut, HighCut };
    void updateLowCutFilter(const ChainSettings& chainSettings);
    void updateHighCutFilter(const ChainSettings& chainSettings);
    void updateFilters();
    
    void clearChannels(AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels, int numSamplesPerChannel);
    
    //Gestion principal del efecto
    void halfspeed(AudioBuffer<float>& audioBuffer, std::vector<float>& writeBuffer, int numChannel, int numSamples, unsigned& writeBufferPosition, unsigned& readBufferPosition, int& amountOfNeededSamples, int& hasToFadeIn);
    void resetHalfspeed();
    void muteAudio(AudioBuffer<float> & buffer, int numChannels, int numSamples);
    
    double currentSampleRate; // == Samples per second
    
    //Variables para el halftime MAIN
    //int amountOfNeededSamplesMain = 0; //cantidad de samples necesarios para realizar el efecto; sera /2 de los samples del fragmento
    std::vector<float> buffer0Main;
    std::vector<float> buffer1Main;
    //Punteros de posicion para el canal L
    unsigned writeBufferPosition0Main = 0;
    unsigned readBufferPosition0Main = 0;
    //Punteros de posicion para el canal R
    unsigned writeBufferPosition1Main = 0;
    unsigned readBufferPosition1Main = 0;
    
    //Variables para el halftime AUX
    //int amountOfNeededSamplesAux = 0;
    std::vector<float> buffer0Aux;
    std::vector<float> buffer1Aux;
    //Punteros de posicion para el canal L Aux
    unsigned writeBufferPosition0Aux = 0;
    unsigned readBufferPosition0Aux = 0;
    //Punteros de posicion para el canal R Aux
    unsigned writeBufferPosition1Aux = 0;
    unsigned readBufferPosition1Aux = 0;
    
    bool hasCalculatedWaitingCycles = false;
    void calculateRemainingWaitingCycles(double timeDivision);
    int64 remainingWaitingCycles = 0;
    
    dsp::DryWetMixer<float> dryWetMixer;    //Esta clase facilita la mezcla de dos señales de audio
                                            //La utilizamos para combinar las señales DRY y WET
    
    int hasToFadeInMain = 0;    //Inicializamos a true para que tambien haga FADE IN en el primer intervalo
                                //Lo inicializamos como numero en vez de bool para que pueda afectar a los dos canales L y R
    int hasToFadeInAux = 0;
    
    dsp::DryWetMixer<float> dryWetTabMixer;
    
    //Funcion que recibe un buffer como entrada y devuelve un nuevo buffer independiente por salida
    AudioBuffer<float> duplicateBuffer(AudioBuffer<float>& buffer, int numChannels, int numSamples);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TFGAudioProcessor)
    
};
