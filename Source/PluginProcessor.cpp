/*
 =====================================================================================

    Este fichero contiene código que interviene en la creación de la GUI del plugin.

 =====================================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

TFGAudioProcessor::TFGAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{ }

TFGAudioProcessor::~TFGAudioProcessor() { }

const String TFGAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool TFGAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TFGAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TFGAudioProcessor::isMidiEffect() const {
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TFGAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int TFGAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TFGAudioProcessor::getCurrentProgram() {
    return 0;
}

void TFGAudioProcessor::setCurrentProgram (int index) { }

const String TFGAudioProcessor::getProgramName (int index) {
    return {};
}

void TFGAudioProcessor::changeProgramName (int index, const String& newName) {
}

void TFGAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    
    //Configuración de los filtros de paso bajo y paso alto
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    updateFilters();
    
}

void TFGAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TFGAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TFGAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    
    if (isPluginOn) {
        
        //En primer lugar, al comenzar a procesar el audio obtenemos el valor actual de BPM
        playHead = this->getPlayHead();
        if (playHead != nullptr) {
            playHead->getCurrentPosition(cpi);
        }
        
        int channelDataArraySize = buffer.getNumSamples();
        
        //En esta parte se va a gestionar cuando se activa y desactiva el efecto
        double beatLength = 60 / cpi.bpm; // Duracion de un pulso; la necesitamos para hacer el halftime mas basico.
        double compas = beatLength * 4 * effectFactor; //Duracion de un compas de 4/4 (para ello hacer *4)
        double currentTimePosition = cpi.timeInSeconds;
        
        int currentNumProcessBlocks = numProcessBlocks * effectFactor;
        //TODO: habra que tener en cuenta de alguna manera, que si cambia el effectFactor durante la reproduccion, el programa tendra que cambiar el procesado para que el sonido se adapte al nuevo efecto.

        
        if (currentTimePosition == 0)
            initializeBlockBufferArray(channelDataArraySize, currentNumProcessBlocks);
        else {
            if (fmod(currentTimePosition, compas) < modError) {
                if (!hasToProcessBlock) destroyBlockBufferArray();
                else initializeBlockBufferArray(channelDataArraySize, currentNumProcessBlocks);
            }
        }
        
        ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
        
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, channelDataArraySize);
        
        //** Ahora mismo procesa 39 o 40 bloques, habra que afinar eso porque yo creo que son 40 los que realmente procesa.
        //Hay que gestionarlo de manera que esos 40 bloques que procesa, se conviertan en 80 bloques finales con halftime.
        
        //Si el plugin debe activar el efecto, entrará en este IF
        if (hasToProcessBlock) {
            
            //DBG("processed block: " + std::to_string(currentBlock));
            ++currentBlock;
            
            //En primer lugar, vamos a escribir en el buffer el bloque actual,
            //y avanzaremos al mismo tiempo el puntero de escritura.
            for (int channel = 0; channel < totalNumInputChannels; ++channel) {
                auto* channelData = buffer.getWritePointer (channel);
                for (int sample = 0; sample < channelDataArraySize; ++sample) {
                    blockBufferArray[writeBufferPosition] = channelData[sample];
                    ++writeBufferPosition;
                }
            }
            
        }
        
        //Ahora, tenemos que ir leyendo todos aquellos bloques que hemos ido acumulando en el buffer.
        //Iremos actualizando el valor de readBufferPosition para ir avanzando por el buffer en cada iteracion.
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer (channel);
            
            //Este es el caso base, en el que comienzo procesando y duplicando la 1era mitad del 1er bloque.
            //Ahora lo que hay que hacer es pasar la otra mitad al siguiente bloque, para que la procese.
            
            int currentInterpolationIndex = 0;
            for (int i = readBufferPosition; i < readBufferPosition + channelDataArraySize / 2; ++i) {

                channelData[currentInterpolationIndex] = blockBufferArray[i];
                channelData[currentInterpolationIndex + 1] = (blockBufferArray[i] + blockBufferArray[i + 1])/2; //IMPORTANTE: hay que interpolar
                currentInterpolationIndex += 2;
            }
            readBufferPosition += channelDataArraySize / 2;
        }
        
        //Llamamos a la función que gestiona los filtros de paso bajo y paso alto
        handleFilters(buffer);
        
        //Por último, gestionamos el slider del output, que controla el volumen con el que sale el audio del plugin
        handleOutputGain(totalNumInputChannels, buffer);
        
    }
}

void TFGAudioProcessor::initializeBlockBufferArray(int channelDataArraySize, int currentNumProcessBlocks) {
    
    //IMPORTANTE: hacer buffer circular
    
    //Esta funcion se encarga de inicializar el buffer que almacenara los bloques totales a procesar
    //Al ponerse a true, debemos preparar todo para comenzar a interpolar los bloques que nos llegan.
    hasToProcessBlock = true;
    currentBlock = 0;
    // De momento, inicializamos el array preparado para 40 bloques
    //Habra que ver como puedo sacar ese numero de manera dinamica
    blockBufferArray = new float[channelDataArraySize * currentNumProcessBlocks * 2]; //Multiplicamos por 2 debido al canal Izquierdo y Derecho
    readBufferPosition = 0;
    writeBufferPosition = 0;
}

void TFGAudioProcessor::destroyBlockBufferArray() {
    //Esta funcion se encarga de limpiar la memoria ocupada por el array del buffer
    //Ademas, indica al plugin que no debe almacenar mas bloques
    hasToProcessBlock = false;
    //Borramos el array y dejamos el puntero a null
    delete [] blockBufferArray;
    blockBufferArray = NULL;
}

void TFGAudioProcessor::handleOutputGain(int totalNumInputChannels, AudioBuffer<float> & buffer) {
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer (channel);
        int channelDataArraySize = buffer.getNumSamples();
        
        for (int sample = 0; sample < channelDataArraySize; ++sample) {
            //El slider muestra valores en dB, pero para procesar esos valores tenemos que encontrar su
            //equivalente en valores de Gain, asi que utilizamos la funcion decibelsToGain
            channelData[sample] = channelData[sample] * Decibels::decibelsToGain(currentDecibels);
        }
    }
}

void TFGAudioProcessor::handleFilters(AudioBuffer<float> & buffer) {
    
    updateFilters();
    
    dsp::AudioBlock<float> block (buffer);
    auto leftBlock = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);
    
    dsp::ProcessContextReplacing<float> leftContext (leftBlock);
    dsp::ProcessContextReplacing<float> rightContext (rightBlock);
    
    leftChain.process (leftContext);
    rightChain.process (rightContext);
}

void TFGAudioProcessor::barButtonChangeCallback(float newBarValue) {
    if(effectFactor != newBarValue)
        effectFactor = newBarValue;
}

void TFGAudioProcessor::pluginOnOffButtonCallback() {
    isPluginOn = isPluginOn ? false : true;
}

bool TFGAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TFGAudioProcessor::createEditor() {
    return new TFGAudioProcessorEditor (*this);
}

void TFGAudioProcessor::getStateInformation (MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TFGAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new TFGAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout TFGAudioProcessor::createParameterLayout() {
    
    AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add (std::make_unique<AudioParameterFloat> ("Low Cut Frequency",
                                                             "Low Cut Frequency",
                                                             NormalisableRange<float> (20.f, 20000.f, 1.f, 0.25f),
                                                             20.f,
                                                             String(),
                                                             AudioProcessorParameter::genericParameter,
                                                             [](float value, int) { return (value < 1000.0f) ?
                                                                 String (value, 0) + " Hz" :
                                                                 String (value / 1000.0f, 1) + " kHz"; },
                                                             nullptr));
    
    layout.add (std::make_unique<AudioParameterFloat> ("High Cut Frequency",
                                                             "High Cut Frequency",
                                                             NormalisableRange<float> (20.f, 20000.f, 1.f, 0.25f),
                                                             20000.f,
                                                             String(),
                                                             AudioProcessorParameter::genericParameter,
                                                             [](float value, int) { return (value < 1000.0f) ?
                                                                 String (value, 0) + " Hz" :
                                                                 String (value / 1000.0f, 1) + " kHz"; },
                                                             nullptr));
    
    return layout;
}

TFGAudioProcessor::ChainSettings TFGAudioProcessor::getChainSettings(AudioProcessorValueTreeState& apvts) {
    TFGAudioProcessor::ChainSettings settings;
    settings.lowCutFreq = apvts.getRawParameterValue ("Low Cut Frequency")->load();
    settings.highCutFreq = apvts.getRawParameterValue ("High Cut Frequency")->load();
    return settings;
}

void TFGAudioProcessor::updateLowCutFilter (const ChainSettings& chainSettings) {
    auto lowCutCoefficients = dsp::IIR::Coefficients<float>::makeHighPass (getSampleRate(), chainSettings.lowCutFreq, defaultQ);
    *leftChain.get<ChainPositions::LowCut>().coefficients = *lowCutCoefficients;
    *rightChain.get<ChainPositions::LowCut>().coefficients = *lowCutCoefficients;
}

void TFGAudioProcessor::updateHighCutFilter (const ChainSettings& chainSettings) {
    auto highCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (getSampleRate(), chainSettings.highCutFreq, defaultQ);
    *leftChain.get<ChainPositions::HighCut>().coefficients = *highCutCoefficients;
    *rightChain.get<ChainPositions::HighCut>().coefficients = *highCutCoefficients;
}

void TFGAudioProcessor::updateFilters() {
    auto chainSettings = getChainSettings (apvts);
    updateLowCutFilter (chainSettings);
    updateHighCutFilter (chainSettings);
}
