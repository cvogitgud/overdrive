/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string>
#include <iostream>

//==============================================================================
TubeSchkreamerAudioProcessor::TubeSchkreamerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMS", createParameterLayout()), highPassFilter(TubeSchkreamerEnums::FilterType::Highpass),
                           lowPassFilter(TubeSchkreamerEnums::FilterType::Lowpass),
                           antiAliasingFilter(juce::dsp::FilterDesign<float>::designFIRLowpassWindowMethod(10000.0f, 44000.0f, 21, juce::dsp::WindowingFunction<float>::hamming))
#endif
{
    treeState.addParameterListener("POWER", this);
    treeState.addParameterListener("PREGAIN", this);
    treeState.addParameterListener("LOWPASSCUTOFF", this);
    treeState.addParameterListener("VOLUME", this);
    
}

TubeSchkreamerAudioProcessor::~TubeSchkreamerAudioProcessor()
{
    treeState.removeParameterListener("POWER", this);
    treeState.removeParameterListener("PREGAIN", this);
    treeState.removeParameterListener("LOWPASSCUTOFF", this);
    treeState.removeParameterListener("VOLUME", this);
}

//==============================================================================
const juce::String TubeSchkreamerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TubeSchkreamerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TubeSchkreamerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TubeSchkreamerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TubeSchkreamerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TubeSchkreamerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TubeSchkreamerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TubeSchkreamerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TubeSchkreamerAudioProcessor::getProgramName (int index)
{
    return {};
}

void TubeSchkreamerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TubeSchkreamerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    int numChannels = getTotalNumOutputChannels();
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    overdrive.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
    
    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
    highPassFilter.updateCutoff(highPassCutoff);
    
    lowPassFilter.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
    
    antiAliasingFilter.reset();
    antiAliasingFilter.prepare(spec);
    
    volume.reset();
    volume.prepare(spec);
    volume.setRampDurationSeconds(0.02f);
    volume.setGainLinear(treeState.getRawParameterValue("VOLUME")->load());
    
    updateParameters();
}

void TubeSchkreamerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TubeSchkreamerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void TubeSchkreamerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clears buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (powerOn){
        juce::dsp::AudioBlock<float> block {buffer};

        highPassFilter.process(buffer);
        antiAliasingFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
        
        overdrive.processBlock(block);
        
        volume.process(juce::dsp::ProcessContextReplacing<float> (block));
        lowPassFilter.process(buffer);
    }
}



juce::AudioProcessorValueTreeState::ParameterLayout TubeSchkreamerAudioProcessor::createParameterLayout (){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    const float minPregain = 1.0f;
    const float maxPregain = 50.0f;
    const float defaultPregain = 10.0f;
    
    const float minFreq = 1500.0f;
    const float maxFreq = 10000.0f;
    const float defaultLowPassCutoff = (maxFreq + minFreq) / 2.0f;
    
    const float minVolume = 0.0f;
    const float maxVolume = 2.0f;
    const float defaultVolume = 0.8f;
    
    auto power = std::make_unique<juce::AudioParameterBool>(juce::ParameterID("POWER", 1), "Power", true);
    
    auto pregain = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PREGAIN", 1), "OVERDRIVE", juce::NormalisableRange<float>(minPregain, maxPregain, 0.01f), defaultPregain);
    
    auto lowPassCutOff = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LOWPASSCUTOFF", 1), "TONE", juce::NormalisableRange<float>(minFreq, maxFreq, 0.01f, 0.3f), defaultLowPassCutoff);
    
    auto volume = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("VOLUME", 1), "LEVEL", juce::NormalisableRange<float>(minVolume, maxVolume, 0.01f), defaultVolume);
    
    params.push_back(std::move(power));
    params.push_back(std::move(pregain));
    params.push_back(std::move(lowPassCutOff));
    params.push_back(std::move(volume));
    
    return {params.begin(), params.end()};
}
    
void TubeSchkreamerAudioProcessor::updatePowerOn(){
    powerOn = treeState.getRawParameterValue("POWER")->load();
}
    
void TubeSchkreamerAudioProcessor::updatePregain (){
    overdrive.setGain(treeState.getRawParameterValue("PREGAIN")->load());
}

void TubeSchkreamerAudioProcessor::updateLowPassFilter (){
    const float lowPassCutoff = treeState.getRawParameterValue("LOWPASSCUTOFF")->load();
    lowPassFilter.updateCutoff(lowPassCutoff);
}

void TubeSchkreamerAudioProcessor::updateVolume (){
    volume.setGainLinear(treeState.getRawParameterValue("VOLUME")->load());
}

void TubeSchkreamerAudioProcessor::updateParameters (){
    updatePowerOn();
    updatePregain();
    updateVolume();
    updateLowPassFilter();
}

void TubeSchkreamerAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue){
    if (parameterID.compare("PREGAIN") == 0){
        updatePregain();
    }
    else if (parameterID.compare("LOWPASSCUTOFF") == 0){
        updateLowPassFilter();
    }
    else if (parameterID.compare("VOLUME") == 0){
        updateVolume();
    }
    else if (parameterID.compare("POWER") == 0){
        updatePowerOn();
    }
}


//==============================================================================
bool TubeSchkreamerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TubeSchkreamerAudioProcessor::createEditor()
{
    return new TubeSchkreamerAudioProcessorEditor (*this);
}

//==============================================================================
void TubeSchkreamerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TubeSchkreamerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TubeSchkreamerAudioProcessor();
}
