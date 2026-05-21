/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocaloneAudioProcessorEditor::VocaloneAudioProcessorEditor (VocaloneAudioProcessor& p)
    : AudioProcessorEditor (&p)
      , audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

VocaloneAudioProcessorEditor::~VocaloneAudioProcessorEditor()
{
}

//==============================================================================
void VocaloneAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::aqua);
    g.setFont (juce::Font("Leelawadee UI", 20.0f, juce::Font::plain));
    g.drawFittedText (juce::String::fromUTF8("สวัสดีซาวด์") + juce::String::fromUTF8(u8"\u2764"), getLocalBounds(), juce::Justification::centred, 1);

}

void VocaloneAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    // วางไว้ตรงกลางขนาด 200x100
}

