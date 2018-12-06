/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#pragma once

#include "Global.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

/**
 * Implements a custom component including a rotary
 * gain slider and phase inversion toggle button.
 */
class BiquadPluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    //==============================================================================
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    //==============================================================================
    BiquadPluginAudioProcessorEditor (BiquadPluginAudioProcessor& owner);
    ~BiquadPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    Label freqLabel;
    Slider freqSlider;
    std::unique_ptr<SliderAttachment> freqAttachment;

    Label qLabel;
    Slider qSlider;
    std::unique_ptr<SliderAttachment> qAttachment;

    //==============================================================================

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BiquadPluginAudioProcessor& getProcessor() const
    {
        return static_cast<BiquadPluginAudioProcessor&> (processor);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiquadPluginAudioProcessorEditor)
};
