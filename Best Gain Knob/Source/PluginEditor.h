#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/** Custom LookAndFeel class to draw the Red Neve-style knob */
class RedNeveKnob : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        // Adjust radius to fit within the slider bounds
        auto radius = (float)juce::jmin (width / 2, height / 2) - 10.0f;
        auto centreX = (float)x + (float)width  * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // 1. Draw the Main Knob Body (Classic Neve Red)
        g.setColour (juce::Colour(0xFF8B0000));
        g.fillEllipse (rx, ry, rw, rw);

        // 2. Add 3D Depth (Gradient and subtle rim)
        g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(0.15f), centreX, ry,
                                               juce::Colours::black.withAlpha(0.3f), centreX, ry + rw, false));
        g.fillEllipse(rx, ry, rw, rw);

        // 3. Draw the Pointer (The white indicator line)
        juce::Path p;
        auto pointerLength = radius * 0.8f;
        auto pointerThickness = 3.5f;
        
        // Shape of the pointer starting from the center outward
        p.addRoundedRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength, 1.0f);
        
        g.setColour (juce::Colours::antiquewhite);
        g.fillPath (p, juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        
        // 4. Subtle Outer Shadow/Ring for realism
        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.drawEllipse(rx, ry, rw, rw, 1.5f);
    }
};

//==============================================================================
class BestGainKnobAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BestGainKnobAudioProcessorEditor (BestGainKnobAudioProcessor&);
    ~BestGainKnobAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BestGainKnobAudioProcessor& audioProcessor;

    // Instance of our custom red knob look
    RedNeveKnob redLookAndFeel;

    juce::Slider gainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BestGainKnobAudioProcessorEditor)
};
