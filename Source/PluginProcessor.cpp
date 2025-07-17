/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessor::OverdriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMS", createParameterLayout()), highPassFilter(), lowPassFilter()
#endif
{
    treeState.addParameterListener("LOWPASSCUTOFF", this);
    treeState.addParameterListener("PREGAIN", this);
}

OverdriveAudioProcessor::~OverdriveAudioProcessor()
{
    treeState.removeParameterListener("LOWPASSCUTOFF", this);
    treeState.removeParameterListener("PREGAIN", this);
}

//==============================================================================
const juce::String OverdriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OverdriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OverdriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OverdriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OverdriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdriveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OverdriveAudioProcessor::getProgramName (int index)
{
    return {};
}

void OverdriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OverdriveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    lowPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    
    highPassFilter.updateParameters(highPassCutoff);
    updateParameters();
}

void OverdriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdriveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OverdriveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clears buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block {buffer};

    highPassFilter.process(buffer);
    
    // Oversampling Anti-Aliasing
    
    // Sample Processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (int sample = 0; sample < block.getNumSamples(); ++sample){
            float input = channelData[sample] * pregain;
//            channelData[sample] = udoDistortion(input);
            channelData[sample] = std::tanh(input);
        }
    }
    
    lowPassFilter.process(buffer);
}

float OverdriveAudioProcessor::udoDistortion(float input){
    
    // turns out middle branch, signInput * blah is the culprit
    // for absolutely SMASHING the output
    // when signInput constantly = 1.0f or -1.0f, it's fine
    // -1.0f is noticeably crunchier
    
    float output = 0.0f;
    float absInput = std::fabs(input);
//    float signInput = (input >= 0) ? 1.0f : -1.0f;
    float signInput = 1.0f;
    
    float threshold = 1/3;

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

//==============================================================================
bool OverdriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdriveAudioProcessor::createEditor()
{
    return new OverdriveAudioProcessorEditor (*this);
}

//==============================================================================
void OverdriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OverdriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdriveAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout OverdriveAudioProcessor::createParameterLayout (){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // i'd like to set this minFreq to highPassCutoff so that if i ever change highPassCutoff, this slider will also change
    // but for some reason, setting it to highPassCutoff results in the cutoff frequency turning to 0 when slider is at
    // minimum position (far left).
    // also i need to document bugs in places better than comments :/
    const float minFreq = 500.0f;
    const float maxFreq = 22000.0f;
    const float defaultLowPassCutoff = 8000.0f;
    
    const float minPregain = 1.0f;
    const float maxPregain = 20.0f;
    const float defaultPregain = 1.0f;
    
    auto lowPassCutOff = std::make_unique<juce::AudioParameterFloat>("LOWPASSCUTOFF", "Low-pass Cutoff", juce::NormalisableRange<float>(minFreq, maxFreq, 0.01f), defaultLowPassCutoff);
    
    auto pregain = std::make_unique<juce::AudioParameterFloat>("PREGAIN", "Pre-gain", juce::NormalisableRange<float>(minPregain, maxPregain, 0.01f), defaultPregain);
    
    params.push_back(std::move(lowPassCutOff));
    params.push_back(std::move(pregain));
    
    return {params.begin(), params.end()};
}

void OverdriveAudioProcessor::updateLowPassFilter (){
    const float lowPassCutoff = treeState.getRawParameterValue("LOWPASSCUTOFF")->load();
    lowPassFilter.updateParameters(lowPassCutoff);
}

void OverdriveAudioProcessor::updatePregain (){
    pregain = treeState.getRawParameterValue("PREGAIN")->load();
}

void OverdriveAudioProcessor::updateParameters (){
    updatePregain();
    updateLowPassFilter();
}

void OverdriveAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue){
    updateParameters();
}
