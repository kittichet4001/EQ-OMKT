#pragma once
#include <JuceHeader.h>

class MyComponent : public juce::Component
{
public:
    MyComponent() {}

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::lightblue); // พื้นหลังสีฟ้า
        g.setColour(juce::Colours::black);
        g.drawText("Hello JUCE!", getLocalBounds(), juce::Justification::centred);
    }

    void resized() override {
        // จัดการ Layout เมื่อขนาด Component เปลี่ยน
    }
};