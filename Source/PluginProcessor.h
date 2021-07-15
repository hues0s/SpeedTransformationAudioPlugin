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
    float currentDecibels { 0.0 };
    bool hasToProcessBlock { true };

    int currentBlock { 1 };
    float effectFactor { 0.5 };   //Esta variable es la que controla cada cuanto tiempo se aplica el efecto.
                                //Por defecto, lo dejamos en 1/2 compás.
    
    //Esta funcion se encargara de actualizar el valor del parametro que gestiona la duracion del efecto.
    //Se llama desde el Plugin Editor al pulsar un boton
    void barButtonChangeCallback(float newBarValue);
    
    bool isPluginOn = true;
    void pluginOnOffButtonCallback();
    
    int readBufferPosition { 0 };
    int writeBufferPosition { 0 };
    float * blockBufferArray { NULL };
    
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", createParameterLayout() };
    struct ChainSettings {
        float lowCutFreq { 0 };
        float highCutFreq { 0 };
    };
    
    ChainSettings getChainSettings (AudioProcessorValueTreeState& apvts);
    
private:
    
    AudioPlayHead *playHead;
    AudioPlayHead::CurrentPositionInfo cpi;
    
    const double modError = 0.023;  //Este valor es utilizado como margen de error a la hora de apagar y encender
                                    //el procesamiento del audio
    
    const int numProcessBlocks = 80;
    
    const double defaultQ = 0.71;
    
    void initializeBlockBufferArray(int channelDataArraySize, int currentNumProcessBlocks);
    void destroyBlockBufferArray();
    void handleOutputGain(int totalNumInputChannels, AudioBuffer<float> & buffer);
    void handleFilters(AudioBuffer<float> & buffer);
    
    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::IIR::Filter<float>> leftChain, rightChain;
    enum ChainPositions {
        LowCut,
        HighCut
    };
    void updateLowCutFilter (const ChainSettings& chainSettings);
    void updateHighCutFilter (const ChainSettings& chainSettings);
    void updateFilters();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TFGAudioProcessor)
};
