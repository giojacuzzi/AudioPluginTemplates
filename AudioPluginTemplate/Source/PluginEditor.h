/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#pragma once

#include "Global.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

/**
 * Implements a custom component including a rotary
 * gain slider and phase inversion toggle button.
 */
class AudioPluginTemplateAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    //==============================================================================
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    //==============================================================================
    AudioPluginTemplateAudioProcessorEditor (AudioPluginTemplateAudioProcessor& owner);
    ~AudioPluginTemplateAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    Label gainLabel;
    Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    ToggleButton invertButton;
    std::unique_ptr<ButtonAttachment> invertAttachment;

    //==============================================================================

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginTemplateAudioProcessor& getProcessor() const
    {
        return static_cast<AudioPluginTemplateAudioProcessor&> (processor);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginTemplateAudioProcessorEditor)
};
