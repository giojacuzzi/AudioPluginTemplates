/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#include "Global.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BiquadPluginAudioProcessorEditor::BiquadPluginAudioProcessorEditor (BiquadPluginAudioProcessor& processor) :
AudioProcessorEditor (processor)
{
    // Add freq slider
    freqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 30);
    freqLabel.attachToComponent(&freqSlider, false);
    freqLabel.setJustificationType(Justification::centred);
    freqLabel.setFont(Font (14.0f));
    freqLabel.setText("Freq", dontSendNotification);
    addAndMakeVisible(freqLabel);
    freqSlider.setSliderStyle(Slider::Rotary);
    addAndMakeVisible(freqSlider);
    freqAttachment.reset(new SliderAttachment (processor.parameterState, "freq", freqSlider));

    // Add q slider
    qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 30);
    qLabel.attachToComponent(&qSlider, false);
    qLabel.setJustificationType(Justification::centred);
    qLabel.setFont(Font (14.0f));
    qLabel.setText("Q", dontSendNotification);
    addAndMakeVisible(qLabel);
    qSlider.setSliderStyle(Slider::Rotary);
    addAndMakeVisible(qSlider);
    qAttachment.reset(new SliderAttachment (processor.parameterState, "q", qSlider));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizeLimits (200, 200, 800, 800);
    setSize (400, 400);
}

BiquadPluginAudioProcessorEditor::~BiquadPluginAudioProcessorEditor()
{
    //
}

//==============================================================================
void BiquadPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour (Global::colorBackground);
    g.fillAll();
}

void BiquadPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    int sliderWidth = getWidth() / 3.0;
    int sliderHeight = getHeight() / 3.0;

    freqSlider.setBounds(getWidth() / 3.0 - sliderWidth / 2.0, getHeight() / 2.0 - sliderHeight / 2.0, sliderWidth, sliderHeight);
    qSlider.setBounds(2.0 * getWidth() / 3.0 - sliderWidth / 2.0, getHeight() / 2.0 - sliderHeight / 2.0, sliderWidth, sliderHeight);
}
