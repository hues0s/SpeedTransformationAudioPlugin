/*
 =====================================================================================

    Este fichero contiene código que interviene en la creación de la GUI del plugin.

 =====================================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>


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
{
    
    dryWetMixer = dsp::DryWetMixer<float>(0);
    //Establecemos: Volumen de la señal DRY = 1 - Volumen de la señal WET
    dryWetMixer.setMixingRule(dsp::DryWetMixingRule::linear);
    
    //Inicializamos el tab mixer
    dryWetTabMixer = dsp::DryWetMixer<float>(0);
    dryWetTabMixer.setMixingRule(dsp::DryWetMixingRule::linear);
}

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

double TFGAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TFGAudioProcessor::getNumPrograms() { return 1; }

int TFGAudioProcessor::getCurrentProgram() { return 0; }

void TFGAudioProcessor::setCurrentProgram (int index) { }

const String TFGAudioProcessor::getProgramName (int index) { return {}; }

void TFGAudioProcessor::changeProgramName (int index, const String& newName) { }

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

void TFGAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    
    currentSampleRate = sampleRate;
        
    //Configuración de los filtros de paso bajo y paso alto
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    updateFilters();
    
    //Configuración del mixer, que gestiona la mezcla entre las señales Dry(sin efecto) y Wet(con efecto)
    dsp::ProcessSpec spec2;
    spec2.maximumBlockSize = samplesPerBlock;
    spec2.numChannels = 2;
    spec2.sampleRate = sampleRate;
    dryWetMixer.prepare(spec2);
    dryWetTabMixer.prepare(spec2);
    
}

void TFGAudioProcessor::releaseResources() { }

void TFGAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    currentNumSamples = buffer.getNumSamples();
    
    //En primer lugar, obtenemos el objeto de tipo AudioPlayHead, que contiene información sobre el estado y la
    //posición del cursor de reproducción. Según la documentación, esta función sólo se puede llamar desde processBlock()
    playHead = this->getPlayHead();
    if (playHead != nullptr) {
        playHead->getCurrentPosition(cpi);
    }
    
    
    if(cpi.isPlaying) { //Audio en reproducción
        
        //Calculamos si hay que esperar, y cuantos bloques deberia esperar
        //La espera se produce cuando iniciamos la reproduccion fuera del punto de halfspeed,
        //y debemos esperar al siguiente punto para empezar a realizar el efecto.
        //La espera dependerá del intervalo maximo escogido en el MAIN o en el AUX.
        calculateRemainingWaitingCycles(std::max(selectedTimeDivision[0], selectedTimeDivision[1]));
        DBG("remainingcycles: " + std::to_string(remainingWaitingCycles));
        if(remainingWaitingCycles > 0) {
            muteAudio(buffer, totalNumInputChannels, currentNumSamples);
            --remainingWaitingCycles;
        }
        
        else {
            
            //En caso de que tengamos mas canales de salida que canales de entrada, esta funcion limpia los canales
            //de salida que no contengan datos de entrada, para evitar que se produzca ruido de salida.
            clearChannels(buffer, totalNumInputChannels, totalNumOutputChannels, buffer.getNumSamples());
            
            //A continuación, establezco el porcentaje de señal DRY/WET y
            //envio el buffer DRY (sin aplicar efecto) al mixer, para que mas tarde pueda mezclarlo con la señal WET
            dryWetMixer.setWetMixProportion(currentDryWetMix/100.0f);
            dryWetMixer.pushDrySamples(dsp::AudioBlock<float>(buffer));
            
            //Gestionamos el halfspeed EXTRA
            if(isAuxOn){
                //Hay que hacer una copia del buffer para que el AUX pueda crear su version independiente del MAIN
                AudioBuffer<float> auxBuffer = duplicateBuffer(buffer, totalNumInputChannels, currentNumSamples);
                
                //Gestionamos el Halftime AUX
                int amountOfNeededSamplesAux = ((60 * 4 * selectedTimeDivision[1] / cpi.bpm ) * currentSampleRate) / 2;
                //Canal L
                halfspeed(auxBuffer, buffer0Aux, 0, currentNumSamples, writeBufferPosition0Aux, readBufferPosition0Aux, amountOfNeededSamplesAux);
                //Canal R
                halfspeed(auxBuffer, buffer1Aux, 1, currentNumSamples, writeBufferPosition1Aux, readBufferPosition1Aux, amountOfNeededSamplesAux);
                
                
                //Gestionamos el Pan del halftime AUX
                handlePan(auxBuffer, totalNumInputChannels, currentNumSamples, currentAuxPan);
                dryWetTabMixer.setWetMixProportion(currentDryWetTabMix/100.0f);
                dryWetTabMixer.pushDrySamples(dsp::AudioBlock<float>(auxBuffer));//CAMBIAR esto solo es DEMO
            }
            
            //Gestionamos el efecto de halfspeed PRINCIPAL
            //Samples a acumular en cada ciclo de halfspeed
            int amountOfNeededSamplesMain = ((60 * 4 * selectedTimeDivision[0] / cpi.bpm ) * currentSampleRate) / 2;

            //Canal L
            halfspeed(buffer, buffer0Main, 0, currentNumSamples, writeBufferPosition0Main, readBufferPosition0Main, amountOfNeededSamplesMain);
            //Canal R
            halfspeed(buffer, buffer1Main, 1, currentNumSamples, writeBufferPosition1Main, readBufferPosition1Main, amountOfNeededSamplesMain);
            
            //Gestionamos el control de Pan del halftime MAIN
            handlePan(buffer, totalNumInputChannels, currentNumSamples, currentMainPan);
            
            //Tras aplicar el halftime PRINCIPAL, debemos añadir el buffer al TabMix para que lo mezcle con el AUX
            if(isAuxOn){
                dryWetTabMixer.mixWetSamples(dsp::AudioBlock<float>(buffer));
            }
            
            //Llamamos a la función que gestiona los filtros de paso bajo y paso alto
            handleFilters(buffer);
            
            //Tras aplicar el efecto de halfspeed, pasamos la señal WET al mixer para que pueda combinarla con la señal DRY
            dryWetMixer.mixWetSamples(dsp::AudioBlock<float>(buffer));
            
            //Gestionamos el slider del output, que controla el volumen con el que sale el audio del plugin
            handleOutputGain(buffer, totalNumInputChannels, currentNumSamples);
            
        }
        
    }
    else { //Audio en pausa
        resetHalfspeed();
    }

}

void TFGAudioProcessor::calculateRemainingWaitingCycles(double timeDivision) {
    
    if (!hasCalculatedWaitingCycles) { //Solo hay que calcularlo en el primer bloque tras hacer PLAY
        
        // (60 / cpi.bpm) es la duracion de un pulso; la necesitamos para hacer el halftime mas basico.
        // A lo anterior se hace *4 para obtener la duracion de un compas de 4/4.
        int totalLengthSamples = (60 * 4 * timeDivision / cpi.bpm ) * currentSampleRate;
        
        //Almaceno el numero de samples que quedan hasta el siguiente ciclo, para que el halftime
        //comience de manera precisa
        int64 remainingWithoutAprox = totalLengthSamples - (cpi.timeInSamples % totalLengthSamples);
        
        if (remainingWithoutAprox <= totalLengthSamples && remainingWithoutAprox >= (totalLengthSamples - 100)){
            //Justo coincide con el inicio de un ciclo -> NO hay que esperar
            //Aqui habra que jugar con un margen de error para que funcione OK; en este caso, 100 samples de margen
            //Conforme avanzamos en el tiempo, hay un margen de error en el remaining haciendo que decrezca progresivamente
            remainingWaitingCycles = 0;
        }
        else { //El ciclo ya ha empezado -> SI hay que esperar
            //El numero de ciclos de processBlock() que debe esperar depende del punto de inicio de reproduccion
            //Si justo coincide con el inicio del bloque, debera esperar x bloques
            //Si coincide con un punto dentro de un bloque, debera esperar x bloques + 1, debido al bloque que ya ha empezado.
            remainingWaitingCycles = (remainingWithoutAprox % currentNumSamples) == 0 ? remainingWithoutAprox/currentNumSamples : remainingWithoutAprox/currentNumSamples + 1;
        }
        hasCalculatedWaitingCycles = true;
    }
}

void TFGAudioProcessor::selectorListener(double changedTimeDivision, int index) {
    resetHalfspeed();
    selectedTimeDivision[index] = changedTimeDivision;
}

void TFGAudioProcessor::resetHalfspeed() {
    //Halftime MAIN
    //amountOfNeededSamplesMain = 0;
    buffer0Main.clear();
    buffer1Main.clear();
    writeBufferPosition0Main = 0;
    readBufferPosition0Main = 0;
    writeBufferPosition1Main = 0;
    readBufferPosition1Main = 0;
    
    //Halftime AUX
    //amountOfNeededSamplesAux = 0;
    buffer0Aux.clear();
    buffer1Aux.clear();
    writeBufferPosition0Aux = 0;
    readBufferPosition0Aux = 0;
    writeBufferPosition1Aux = 0;
    readBufferPosition1Aux = 0;
    
    hasCalculatedWaitingCycles = false;
}

void TFGAudioProcessor::halfspeed(AudioBuffer<float>& audioBuffer, std::vector<float>& writeBuffer, int numChannel, int numSamples, unsigned& writeBufferPosition, unsigned& readBufferPosition, int& amountOfNeededSamples) {
    
    unsigned currentFadeLength =
        (currentSmoothMs * currentSampleRate) / 1000 // currentSampleRate equivale a las muestras por segundo = 1000ms
        / 2; //Dividimos /2 porque el fade se va a realizar antes que la duplicacion de samples
   
    
    //Escribimos en el write buffer el bloque de audio entrante
    if (writeBufferPosition < amountOfNeededSamples) {
        
        //La variable writeCount almacenara el total de samples a escribir en el writeBuffer
        //Leeremos siempre numSamples, a excepcion de al final, ya que es posible que debamos leer menos de un bloque entero
        auto writeCount = amountOfNeededSamples - writeBufferPosition > numSamples ? numSamples : amountOfNeededSamples - writeBufferPosition;
        auto readPointer = audioBuffer.getReadPointer(numChannel);
        writeBuffer.insert(writeBuffer.end(), readPointer , readPointer + writeCount);
        writeBufferPosition += writeCount;
    }
    
    //Leemos lo que esta almacenado en el writeBuffer para escribir en el buffer de salida
    auto writePointer = audioBuffer.getWritePointer(numChannel);
    int remaining = numSamples;
    
    //Obtenemos el elemento anterior para poder interpolar con el elemento actual
    auto last = (readBufferPosition == 0 ? 0 : writeBuffer[readBufferPosition - 1]);
        
    while (remaining >= 2) {
        remaining -= 2; //Por cada sample que leemos del writeBuffer, escribimos 2 samples en el buffer de salida
        
        float fadeFactor = 1;
        //Fade IN
        if (readBufferPosition <= currentFadeLength) {
            fadeFactor = readBufferPosition;
            fadeFactor /= currentFadeLength;
        }
        //Fade OUT
        if (amountOfNeededSamples - readBufferPosition <= currentFadeLength) {
            fadeFactor = (amountOfNeededSamples - readBufferPosition);
            fadeFactor /= currentFadeLength;
        }
         
        //Leemos del writeBuffer y realizamos la interpolacion
        *writePointer = (writeBuffer[readBufferPosition] - last)/2 * fadeFactor;
        writePointer++;
        *writePointer = writeBuffer[readBufferPosition] * fadeFactor;
        writePointer++;
        last = writeBuffer[readBufferPosition];
        
        //Avanzamos una posicion de lectura en el writeBuffer
        readBufferPosition++;
        
        //Si hemos acabado de leer el writeBuffer, entramos aqui y limpiamos
        if (readBufferPosition >= writeBuffer.size()) {
            writeBuffer.clear();
            writeBufferPosition = 0;
            readBufferPosition = 0;
                        
            // Si quedan datos sin sobrescribir que pertenecen al siquiente beat, los guardamos para terminar de leerlos.
            if (remaining) {
                int copy_amount = remaining > amountOfNeededSamples ? amountOfNeededSamples : remaining;
                auto readPointerRemainingSamples = audioBuffer.getReadPointer(numChannel) + numSamples - remaining;
                writeBuffer.insert(writeBuffer.end(), readPointerRemainingSamples , readPointerRemainingSamples + copy_amount);
                writeBufferPosition += remaining;
            }
        }
    }
    
}

void TFGAudioProcessor::clearChannels(AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels, int numSamplesPerChannel){
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamplesPerChannel);
}

void TFGAudioProcessor::muteAudio(AudioBuffer<float> & buffer, int numChannels, int numSamples) {
    for (int channel = 0; channel < numChannels; ++channel) {
        auto * writePointer = buffer.getWritePointer (channel);
        for (int sample = 0; sample < numSamples; ++sample) writePointer[sample] = 0;
    }
}

void TFGAudioProcessor::handleOutputGain(AudioBuffer<float> & buffer, int numChannels, int numSamples) {
    for (int channel = 0; channel < numChannels; ++channel) {
        auto * writePointer = buffer.getWritePointer (channel);
        for (int sample = 0; sample < numSamples; ++sample) {
            //El slider muestra valores en dB, pero para procesar esos valores tenemos que encontrar su
            //equivalente en valores de Gain, asi que utilizamos la funcion decibelsToGain
            writePointer[sample] = writePointer[sample] * Decibels::decibelsToGain(currentDecibels);
        }
    }
}

void TFGAudioProcessor::handlePan(AudioBuffer<float> & buffer, int numChannels, int numSamples, float currentPan) {
    if(numChannels == 2){
        double panParameter = M_PI*(currentPan + 1)/4;
        
        auto * writePointer = buffer.getWritePointer (0);
        for (int sample = 0; sample < numSamples; ++sample)
            writePointer[sample] = writePointer[sample] * cos(panParameter);
        writePointer = buffer.getWritePointer (1);
        for (int sample = 0; sample < numSamples; ++sample)
            writePointer[sample] = writePointer[sample] * sin(panParameter);
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

void TFGAudioProcessor::auxOnOffButtonCallback(bool isAuxOn) {
    resetHalfspeed();
    this->isAuxOn = isAuxOn;
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

AudioBuffer<float> TFGAudioProcessor::duplicateBuffer(AudioBuffer<float>& buffer, int numChannels, int numSamples){
    AudioBuffer<float> returnBuffer = AudioBuffer<float>(numChannels, numSamples);
    for (int channel = 0; channel < numChannels; ++channel) {
        auto * writePointer = returnBuffer.getWritePointer (channel);
        for (int sample = 0; sample < numSamples; ++sample) {
            writePointer[sample] = buffer.getSample(channel, sample);
        }
    }
    return returnBuffer;
}
