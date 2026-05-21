/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class VocaloneAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VocaloneAudioProcessor();
    ~VocaloneAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void reset() override;
    void OMKTSound(juce::AudioBuffer<float>& buf);

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // 1. Foundation: High Pass Filter (Fixed at 80-100Hz)
// 1. นิยาม Filter แบบที่เป็น Stereo แล้ว (Duplicator อยู่ข้างใน)
    using StereoFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

    // 2. เอา StereoFilter มาต่อกันเป็น Chain
    using StereoChain = juce::dsp::ProcessorChain<StereoFilter, StereoFilter, StereoFilter,StereoFilter>;

    StereoChain fchain;   
    float OMKT_Q = 100.0f;
    juce::AudioBuffer<float> wetBuffer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocaloneAudioProcessor)
};
