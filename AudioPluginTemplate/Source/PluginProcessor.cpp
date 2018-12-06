/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginTemplateAudioProcessor::AudioPluginTemplateAudioProcessor() :
    #ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
                    .withInput  ("Input",  AudioChannelSet::stereo(), true)
    #endif
                    .withOutput ("Output", AudioChannelSet::stereo(), true)
    #endif
                    ),
    #endif
    parameterState (*this, nullptr, Identifier ("valueTreeState"),
    {
        // Create parameters here ...
        std::make_unique<AudioParameterFloat> ("gain",            // ID
                                               "Gain",            // name
                                               0.0f,              // min value
                                               1.0f,              // max value
                                               0.5f),             // default value
        std::make_unique<AudioParameterBool> ("invertPhase",      // ID
                                              "Invert Phase",     // name
                                              false)              // default value
    })
{
    // Store pointers to our parameters to
    // use later in the the process function
    parameterGain = parameterState.getRawParameterValue ("gain");
    parameterPhase = parameterState.getRawParameterValue ("invertPhase");
}

AudioPluginTemplateAudioProcessor::~AudioPluginTemplateAudioProcessor()
{
    //
}

//==============================================================================
const String AudioPluginTemplateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginTemplateAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginTemplateAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginTemplateAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginTemplateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginTemplateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginTemplateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginTemplateAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPluginTemplateAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPluginTemplateAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioPluginTemplateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    auto phase = *parameterPhase < 0.5f ? 1.0f : -1.0f;
    previousGain = *parameterGain * phase;
}

void AudioPluginTemplateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginTemplateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void AudioPluginTemplateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);
        // ..do something to the data...
    }

    // Apply phase parameter
    auto phase = *parameterPhase < 0.5f ? 1.0f : -1.0f;
    auto currentGain = *parameterGain * phase;

    // Apply smoothed gain parameter
    if (currentGain == previousGain) {
        buffer.applyGain (currentGain);
    } else {
        buffer.applyGainRamp (0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
}

//==============================================================================
bool AudioPluginTemplateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPluginTemplateAudioProcessor::createEditor()
{
    return new AudioPluginTemplateAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginTemplateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Store an xml representation of our state.
    std::unique_ptr<XmlElement> xmlState (parameterState.copyState().createXml());
    if (xmlState.get() != nullptr)
        copyXmlToBinary (*xmlState, destData);
}

void AudioPluginTemplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore our plug-in's state from the xml representation stored in the above
    // method.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        parameterState.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginTemplateAudioProcessor();
}
