/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocaloneAudioProcessor::VocaloneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

VocaloneAudioProcessor::~VocaloneAudioProcessor()
{
}

//==============================================================================
const juce::String VocaloneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VocaloneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VocaloneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VocaloneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VocaloneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VocaloneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VocaloneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VocaloneAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VocaloneAudioProcessor::getProgramName (int index)
{
    return {};
}

void VocaloneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void VocaloneAudioProcessor::OMKTSound(juce::AudioBuffer<float>& buf) {
    juce::dsp::AudioBlock<float> block(buf);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // 2. Step 1: ตัดขยะ (HPF) - รากฐานต้องคลีน
    //sc2.process(context);
    fchain.process(context);

}

//==============================================================================
void VocaloneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // 1. เตรียมค่า Spec สำหรับ DSP
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    wetBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock);

    //secretFilter.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 500.0f, 250.0f,juce::Decibels::decibelsToGain(-1.0f));
// เข้าถึง Reference ของ chain ข้างในตรงๆ
// 3. เวลาตั้งค่า (อัปเดตที่ .state ของแต่ละ Stage)
    fchain.get<0>().state = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 200.0f);
    fchain.get<1>().state = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 500.0f);    // 2. สั่งปั๊มร่างแยกตามจำนวน Channel
    fchain.get<2>().state = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 1000.0f);    // 2. สั่งปั๊มร่างแยกตามจำนวน Channel
    fchain.get<3>().state = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.0f, 2.0f, juce::Decibels::decibelsToGain(3.0f));   // 2. สั่งปั๊มร่างแยกตามจำนวน Channel
    //fchain.get<3>().state = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderChebyshev1Method;   // 2. สั่งปั๊มร่างแยกตามจำนวน Channel
    // 3. ล้างขยะใน Register (State Variables)
    fchain.prepare(spec);
    fchain.reset();

}

void VocaloneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VocaloneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VocaloneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();// ในจังหวะที่ตั้งค่า SVF
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
        wetBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        // 1. เตรียม Context สำหรับ DSP Chain
#if 1
        OMKTSound(wetBuffer);
#else
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        // 2. Step 1: ตัดขยะ (HPF) - รากฐานต้องคลีน
        svfFilter.process(context);
        secretFilter.process(context);
#endif
    }
    buffer.applyGain(juce::Decibels::decibelsToGain(-3.0f));
    // 4. นำ wetBuffer มาบวกผสม (Mix) เข้ากับ main buffer
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        buffer.addFrom(ch, 0, wetBuffer.getReadPointer(ch), totalNumInputChannels, juce::Decibels::decibelsToGain(-27.75f));
    }
}
void VocaloneAudioProcessor::reset() {
    //hpFilter.reset();
}

//==============================================================================
bool VocaloneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VocaloneAudioProcessor::createEditor()
{
    return new VocaloneAudioProcessorEditor (*this);
}

//==============================================================================
void VocaloneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VocaloneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocaloneAudioProcessor();
}
