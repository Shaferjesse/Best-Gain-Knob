#include "PluginProcessor.h"
#include "PluginEditor.h"

BestGainKnobAudioProcessorEditor::BestGainKnobAudioProcessorEditor (BestGainKnobAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Apply the custom Red Neve LookAndFeel
    gainKnob.setLookAndFeel(&redLookAndFeel);

    // 1. Smooth "Outboard" feel
    gainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainKnob.setMouseDragSensitivity(250);
    
    // Makes it easier to hit 0dB exactly
    gainKnob.setSliderSnapsToMousePosition(false);
    
    addAndMakeVisible(gainKnob);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.treeState, "gain", gainKnob);

    setSize (300, 400); // Taller to fit the branding and scale
}

BestGainKnobAudioProcessorEditor::~BestGainKnobAudioProcessorEditor()
{
    // ALWAYS reset the look and feel to nullptr in the destructor to avoid memory crashes
    gainKnob.setLookAndFeel(nullptr);
}

void BestGainKnobAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background: Dark brushed metal look
    auto background = getLocalBounds().toFloat();
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xFF222222), background.getTopLeft(),
                                           juce::Colour(0xFF0F0F0F), background.getBottomRight(), false));
    g.fillAll();

    // Use the exact center of the UI for the scale marks
    auto center = getLocalBounds().getCentre().toFloat().translated(0, 20);
    float radius = 95.0f;

    g.setColour (juce::Colours::antiquewhite.withAlpha(0.9f));

    // DRAWING THE SCALE: 16 steps total
    // 0 = -20dB, 4 = -10dB, 8 = 0dB (Unity), 12 = +10dB, 16 = +20dB
    for (int i = 0; i <= 16; ++i)
    {
        float angle = juce::jmap((float)i, 0.0f, 16.0f, -2.1f, 2.1f);
        
        bool isMajor = (i % 4 == 0);  // Every 10dB (Large tick + Number)
        bool isMid = (i % 2 == 0);    // Every 5dB (Medium tick)
        
        float tickLength = isMajor ? 15.0f : (isMid ? 10.0f : 5.0f);
        float thickness = isMajor ? 2.5f : 1.0f;

        auto p1 = center.getPointOnCircumference (radius, angle);
        auto p2 = center.getPointOnCircumference (radius + tickLength, angle);

        g.drawLine (p1.getX(), p1.getY(), p2.getX(), p2.getY(), thickness);

        // Draw Numbers for Major Ticks (-20, -10, 0, 10, 20)
        if (isMajor)
        {
            auto labelPos = center.getPointOnCircumference (radius + 35.0f, angle);
            g.setFont (juce::Font("Impact", 20.0f, juce::Font::plain));
            
            int dbValue = (i * 2.5f) - 20;
            juce::String valueText = (dbValue > 0 ? "+" : "") + juce::String(dbValue);
            if (dbValue == 0) valueText = "0";

            g.drawText (valueText, labelPos.getX() - 25, labelPos.getY() - 10, 50, 20, juce::Justification::centred);
        }
    }

    // VINTAGE BRANDING
    g.setColour(juce::Colours::antiquewhite);
    g.setFont(juce::Font("Copperplate", 24.0f, juce::Font::bold));
    g.drawText("JESSE SHAFER", 0, 30, getWidth(), 30, juce::Justification::centred);
    
    g.setFont(juce::Font("Verdana", 11.0f, juce::Font::plain));
    g.setColour(juce::Colours::grey);
    g.drawText("MODEL 1 - GAIN STAGE", 0, 55, getWidth(), 20, juce::Justification::centred);
}

void BestGainKnobAudioProcessorEditor::resized()
{
    // Position the knob so it sits perfectly inside the painted scale
    auto applyLayout = [this]() {
        auto knobArea = getLocalBounds().reduced(100);
        // Move down by 20 to match the scale in the paint function
        gainKnob.setBounds(knobArea.translated(0, 20));
    };

    applyLayout();
}
