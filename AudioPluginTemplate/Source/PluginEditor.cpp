/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#include "Global.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginTemplateAudioProcessorEditor::AudioPluginTemplateAudioProcessorEditor (AudioPluginTemplateAudioProcessor& processor) : AudioProcessorEditor (processor)
{
    // Add gain slider value textbox
    gainSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 40, 30);

    // Add gain slider label
    gainLabel.attachToComponent (&gainSlider, false);
    gainLabel.setJustificationType(Justification::centred);
    gainLabel.setFont (Font (14.0f));
    gainLabel.setText ("Gain", dontSendNotification);
    addAndMakeVisible (gainLabel);

    // Add gain slider
    gainSlider.setSliderStyle(Slider::Rotary);
    addAndMakeVisible (gainSlider);
    gainAttachment.reset (new SliderAttachment (processor.parameterState, "gain", gainSlider));

    // Add phase invert button
    invertButton.setButtonText ("Invert Phase");
    addAndMakeVisible (invertButton);
    invertAttachment.reset (new ButtonAttachment (processor.parameterState, "invertPhase", invertButton));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizeLimits (200, 200, 800, 800);
    setSize (400, 400);
}

AudioPluginTemplateAudioProcessorEditor::~AudioPluginTemplateAudioProcessorEditor()
{
    //
}

//==============================================================================
void AudioPluginTemplateAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour (Global::colorBackground);
    g.fillAll();
}

void AudioPluginTemplateAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    int gainSliderWidth = getWidth() / 2.0;
    int gainSliderHeight = getHeight() / 2.0;
    gainSlider.setBounds(gainSliderWidth / 2.0, gainSliderHeight / 2.0, gainSliderWidth, gainSliderHeight);

    int invertButtonWidth = 100;
    int invertButtonHeight = 100;
    invertButton.setBounds(getWidth() / 2.0 - invertButtonWidth / 2.0, getHeight() / 1.1 - invertButtonHeight / 2.0, invertButtonWidth, invertButtonHeight);
}
