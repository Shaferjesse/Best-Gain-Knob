#include "PluginProcessor.h"
#include "PluginEditor.h"

BestGainKnobAudioProcessorEditor::BestGainKnobAudioProcessorEditor (BestGainKnobAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configure the knob
    gainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(gainKnob);

    // Attach to the processor's treeState
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.treeState, "gain", gainKnob);

    setSize (250, 300);
}

BestGainKnobAudioProcessorEditor::~BestGainKnobAudioProcessorEditor()
{
}

void BestGainKnobAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void BestGainKnobAudioProcessorEditor::resized()
{
    // Using an arrow function for layout to keep IDE lines clean
    auto applyLayout = [this]() {
        gainKnob.setBounds(getLocalBounds().reduced(40));
    };

    applyLayout();
}
