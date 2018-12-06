/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#include <iostream>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BiquadPluginAudioProcessor::BiquadPluginAudioProcessor() :
    #ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
                    .withInput  ("Input",  AudioChannelSet::mono(), true)
    #endif
                    .withOutput ("Output", AudioChannelSet::mono(), true)
    #endif
                    ),
    #endif
    parameterState (*this, nullptr, Identifier ("valueTreeState"),
    {
        // Create parameters here ...
        std::make_unique<AudioParameterFloat> ("freq",            // ID
                                               "Freq",            // name
                                               20.0f,              // min value
                                               20000.0f,              // max value
                                               1000.0f),             // default value
        std::make_unique<AudioParameterFloat> ("q",
                                               "Q",
                                               0.0f,
                                               1.0f,
                                               0.7071f)
    })
{
    // Store pointers to our parameters to
    // use later in the the process function
    parameterFreq = parameterState.getRawParameterValue ("freq");
    parameterQ = parameterState.getRawParameterValue ("q");
}

BiquadPluginAudioProcessor::~BiquadPluginAudioProcessor()
{
    //
}

//==============================================================================
const String BiquadPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BiquadPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BiquadPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BiquadPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BiquadPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BiquadPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int BiquadPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BiquadPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String BiquadPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void BiquadPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BiquadPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize the biquad
    biquad.setFs(sampleRate);
}

void BiquadPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BiquadPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BiquadPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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

    // TODO: use listeners to set these parameters only when they ahve changed
    float fc = *parameterFreq;
    float q = *parameterQ;
    biquad.setFc(fc);
    biquad.setQ(q);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            channelData[sample] = biquad.process(channelData[sample]);
        }
    }
}

//==============================================================================
bool BiquadPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BiquadPluginAudioProcessor::createEditor()
{
    return new BiquadPluginAudioProcessorEditor (*this);
}

//==============================================================================
void BiquadPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Store an xml representation of our state.
    std::unique_ptr<XmlElement> xmlState (parameterState.copyState().createXml());
    if (xmlState.get() != nullptr)
        copyXmlToBinary (*xmlState, destData);
}

void BiquadPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BiquadPluginAudioProcessor();
}
