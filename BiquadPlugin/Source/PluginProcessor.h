/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#pragma once

#include "Biquad.h"
#include "../JuceLibraryCode/JuceHeader.h"

/**
 * Applies gain and phase inversion parameters to audio input.
 */
class BiquadPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    BiquadPluginAudioProcessor();
    ~BiquadPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AudioProcessorValueTreeState parameterState;

private:
    //==============================================================================
    Biquad biquad;

    //==============================================================================
    float* parameterFreq  = nullptr;
    float* parameterQ = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiquadPluginAudioProcessor)
};
