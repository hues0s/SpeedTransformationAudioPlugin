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
    double selectedTimeDivision = { 1 };
    float currentDecibels { 0.0 };
    int currentDryWetMix { 100 };
    
    bool isPluginOn = true;
    void pluginOnOffButtonCallback(bool isPluginOn);
    
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", createParameterLayout() };
    struct ChainSettings {
        float lowCutFreq { 0 };
        float highCutFreq { 0 };
    };
    
    ChainSettings getChainSettings (AudioProcessorValueTreeState& apvts);
    
    
    void mainSelectorListener(double changedTimeDivision);
    
private:
    
    AudioPlayHead *playHead;
    AudioPlayHead::CurrentPositionInfo cpi;
    int currentNumSamples = 0;
    const double defaultQ = 0.71;
    
    void handleOutputGain(AudioBuffer<float> & buffer, int numChannels, int numSamples);
    void handlePan(AudioBuffer<float> & buffer, int numChannels, int numSamples);
    void handleFilters(AudioBuffer<float> & buffer);
    
    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::IIR::Filter<float>> leftChain, rightChain;
    enum ChainPositions { LowCut, HighCut };
    void updateLowCutFilter(const ChainSettings& chainSettings);
    void updateHighCutFilter(const ChainSettings& chainSettings);
    void updateFilters();
    
    void clearChannels(AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels, int numSamplesPerChannel);
    
    //Gestion principal del efecto
    void halfspeed(AudioBuffer<float>& audioBuffer, std::vector<float>& writeBuffer, int numChannel, int numSamples, unsigned& writeBufferPosition, unsigned& readBufferPosition);
    void resetHalfspeed();
    void muteAudio(AudioBuffer<float> & buffer, int numChannels, int numSamples);
    
    double currentSampleRate; // == Samples per second
    int amountOfNeededSamples = 0; //cantidad de samples necesarios para realizar el efecto; sera /2 de los samples del fragmento
    
    std::vector<float> buffer0;
    std::vector<float> buffer1;
    
    //Punteros de posicion para el canal L
    unsigned writeBufferPosition0 = 0;
    unsigned readBufferPosition0 = 0;
    //Punteros de posicion para el canal R
    unsigned writeBufferPosition1 = 0;
    unsigned readBufferPosition1 = 0;
    
    void calculateRemainingWaitingCycles();
    int64 remainingWaitingCycles = 0;
    
    dsp::DryWetMixer<float> dryWetMixer;    //Esta clase facilita la mezcla de dos señales de audio
                                            //La utilizamos para combinar las señales DRY y WET
    
    int hasToFadeIn = 0;    //Inicializamos a true para que tambien haga FADE IN en el primer intervalo
                            //Lo inicializamos como numero en vez de bool para que pueda afectar a los dos canales L y R
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TFGAudioProcessor)
    
};
