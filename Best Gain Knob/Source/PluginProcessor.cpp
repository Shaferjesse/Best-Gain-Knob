
#include "PluginProcessor.h"
#include "PluginEditor.h"

BestGainKnobAudioProcessor::BestGainKnobAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
.withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
),
treeState (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

BestGainKnobAudioProcessor::~BestGainKnobAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout BestGainKnobAudioProcessor::createParameterLayout()
{
std::vector< std::unique_ptr< juce::RangedAudioParameter > > params;
params.push_back(std::make_unique< juce::AudioParameterFloat >("gain", "Gain", -20.0f, 20.0f, 0.0f));
return { params.begin(), params.end() };
}

void BestGainKnobAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}
void BestGainKnobAudioProcessor::releaseResources() {}

void BestGainKnobAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
juce::ScopedNoDenormals noDenormals;
auto totalNumInputChannels  = getTotalNumInputChannels();
auto totalNumOutputChannels = getTotalNumOutputChannels();

for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear (i, 0, buffer.getNumSamples());

float gainDb = *treeState.getRawParameterValue("gain");
float gainScalar = juce::Decibels::decibelsToGain(gainDb);

for (int channel = 0; channel < totalNumInputChannels; ++channel)
{
    buffer.applyGain(channel, 0, buffer.getNumSamples(), gainScalar);
}
}

const juce::String BestGainKnobAudioProcessor::getName() const { return JucePlugin_Name; }
bool BestGainKnobAudioProcessor::acceptsMidi() const { return false; }
bool BestGainKnobAudioProcessor::producesMidi() const { return false; }
bool BestGainKnobAudioProcessor::isMidiEffect() const { return false; }
double BestGainKnobAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int BestGainKnobAudioProcessor::getNumPrograms() { return 1; }
int BestGainKnobAudioProcessor::getCurrentProgram() { return 0; }
void BestGainKnobAudioProcessor::setCurrentProgram (int index) {}
const juce::String BestGainKnobAudioProcessor::getProgramName (int index) { return {}; }
void BestGainKnobAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

bool BestGainKnobAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* BestGainKnobAudioProcessor::createEditor()
{
return new BestGainKnobAudioProcessorEditor (*this);
}

void BestGainKnobAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
auto state = treeState.copyState();
std::unique_ptr< juce::XmlElement > xml (state.createXml());
copyXmlToBinary (*xml, destData);
}

void BestGainKnobAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
std::unique_ptr< juce::XmlElement > xmlState (getXmlFromBinary (data, sizeInBytes));
if (xmlState.get() != nullptr)
if (xmlState->hasTagName (treeState.state.getType()))
treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
return new BestGainKnobAudioProcessor();
}
