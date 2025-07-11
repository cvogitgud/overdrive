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
                       ), treeState(*this, nullptr, "PARAMS", createParameterLayout()),
                          highPassFilter(), lowPassFilter()
#endif
{
    treeState.addParameterListener("HIGHPASSCUTOFF", this);
    treeState.addParameterListener("HIGHPASSRES", this);
    treeState.addParameterListener("LOWPASSCUTOFF", this);
    treeState.addParameterListener("LOWPASSRES", this);
}

OverdriveAudioProcessor::~OverdriveAudioProcessor()
{
    treeState.removeParameterListener("HIGHPASSCUTOFF", this);
    treeState.removeParameterListener("HIGHPASSRES", this);
    treeState.removeParameterListener("LOWPASSCUTOFF", this);
    treeState.removeParameterListener("LOWPASSRES", this);
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
    // lastSampleRate = sampleRate ?
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    // prepare high pass filter
    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    
    
    // prepare low pass filter
    lowPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    
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
    
    // IIR High Pass Filter
    highPassFilter.process(buffer);
    
    // Oversampling Anti-Aliasing
    
    // Processing (probably where i want my distortion class to be)
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    // IIR Low Pass filter
    lowPassFilter.process(buffer);
}

//==============================================================================
bool OverdriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdriveAudioProcessor::createEditor()
{
//    return new OverdriveAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
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
    
    
    auto highPassCutOff = std::make_unique<juce::AudioParameterFloat>("HIGHPASSCUTOFF", "High-pass Cutoff", juce::NormalisableRange<float>(10.0f, 22000.0f, 0.01f, 0.25f), 40.0f);
    auto highPassResonance = std::make_unique<juce::AudioParameterFloat>("HIGHPASSRES", "High-pass Resonance", juce::NormalisableRange<float>(0.10f, 18.0f, 0.01f), 0.1f);
    
    auto lowPassCutOff = std::make_unique<juce::AudioParameterFloat>("LOWPASSCUTOFF", "Low-pass Cutoff", juce::NormalisableRange<float>(10.0f, 22000.0f, 0.01f, 0.25f), 8000.0f);
    auto lowPassResonance = std::make_unique<juce::AudioParameterFloat>("LOWPASSRES", "Low-pass Resonance", juce::NormalisableRange<float>(0.10f, 18.0f, 0.01f), 0.1f);
    
    params.push_back(std::move(highPassCutOff));
    params.push_back(std::move(highPassResonance));
    params.push_back(std::move(lowPassCutOff));
    params.push_back(std::move(lowPassResonance));
    
    return {params.begin(), params.end()};
}

void OverdriveAudioProcessor::updateHighPassFilter (){
    // grab freq and res from treeState
    float highPassCutoff = treeState.getRawParameterValue("HIGHPASSCUTOFF")->load();
    float highPassResonance = treeState.getRawParameterValue("HIGHPASSRES")->load();

    highPassFilter.updateParameters(highPassCutoff, highPassResonance);
}

void OverdriveAudioProcessor::updateLowPassFilter (){
    // grab freq and res from treeState
    float lowPassCutoff = treeState.getRawParameterValue("LOWPASSCUTOFF")->load();
    float lowPassResonance = treeState.getRawParameterValue("LOWPASSRES")->load();

    lowPassFilter.updateParameters(lowPassCutoff, lowPassResonance);
}

void OverdriveAudioProcessor::updateParameters (){
    updateHighPassFilter();
    updateLowPassFilter();
}

void OverdriveAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue){
    updateParameters();
}
