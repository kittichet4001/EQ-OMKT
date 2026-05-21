/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/**
*/
class VocaloneAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VocaloneAudioProcessorEditor (VocaloneAudioProcessor&);
    ~VocaloneAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;   
    void ToneSelChanged();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VocaloneAudioProcessor& audioProcessor;
    juce::Slider midiVolume; // [1]
    juce::AudioParameterChoice* choiceParam;
    MyComponent myCustomComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocaloneAudioProcessorEditor)
};
