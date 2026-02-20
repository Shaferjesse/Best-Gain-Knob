#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BestGainKnobAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BestGainKnobAudioProcessorEditor (BestGainKnobAudioProcessor&);
    ~BestGainKnobAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BestGainKnobAudioProcessor& audioProcessor;

    juce::Slider gainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BestGainKnobAudioProcessorEditor)
};
