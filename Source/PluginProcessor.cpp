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
    treeState.addParameterListener("POWER", this);
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
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    pregain.reset();
    pregain.prepare(spec);
    pregain.setRampDurationSeconds(0.02f);
    pregain.setGainLinear(treeState.getRawParameterValue("PREGAIN")->load());
    
    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    lowPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    
    highPassFilter.updateCutoff(highPassCutoff);
    
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
        pregain.process(juce::dsp::ProcessContextReplacing<float> (block));
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

            for (int sample = 0; sample < block.getNumSamples(); ++sample){
                float input = channelData[sample];
                channelData[sample] = udoDistortion(input);
            }
        }
        
        volume.process(juce::dsp::ProcessContextReplacing<float> (block));
        lowPassFilter.process(buffer);
    }
}

float TubeSchkreamerAudioProcessor::udoDistortion(float input){
    float output = 0.0f;
    float absInput = std::fabs(input);
    float signInput = (input >= 0) ? 1.0f : -1.0f;
    float threshold = 1.0f/3.0f;

    if (absInput < threshold) {
        output = 2.0f * input;
    }
    else if (absInput >= threshold && absInput < (2.0f * threshold)){
        float base = 2.0f - 3.0f * absInput;
        output = signInput * (3.0f - (base * base)) / 3.0f;
    }
    else {
        output = signInput;
    }
    
    return output;
}

juce::AudioProcessorValueTreeState::ParameterLayout TubeSchkreamerAudioProcessor::createParameterLayout (){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    const float minPregain = 1.0f;
    const float maxPregain = 50.0f;
    const float defaultPregain = 10.0f;
    
    const float minFreq = 70.0f;
    const float maxFreq = 10000.0f;
    const float defaultLowPassCutoff = 5000.0f;
    
    const float minVolume = 0.0f;
    const float maxVolume = 2.0f;
    const float defaultVolume = 0.8f;
    
    auto power = std::make_unique<juce::AudioParameterBool>("POWER", "Power", true);
    
    auto pregain = std::make_unique<juce::AudioParameterFloat>("PREGAIN", "OVERDRIVE", juce::NormalisableRange<float>(minPregain, maxPregain, 0.01f), defaultPregain);
    
    auto lowPassCutOff = std::make_unique<juce::AudioParameterFloat>("LOWPASSCUTOFF", "TONE", juce::NormalisableRange<float>(minFreq, maxFreq, 0.01f, 0.3f), defaultLowPassCutoff);
    
    auto volume = std::make_unique<juce::AudioParameterFloat>("VOLUME", "LEVEL", juce::NormalisableRange<float>(minVolume, maxVolume, 0.01f), defaultVolume);
    
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
    pregain.setGainLinear(treeState.getRawParameterValue("PREGAIN")->load());
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
