/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidSideAudioProcessor::MidSideAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       )
{
}

MidSideAudioProcessor::~MidSideAudioProcessor()
{
}

//==============================================================================
const juce::String MidSideAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidSideAudioProcessor::acceptsMidi() const
{
    return false;
}

bool MidSideAudioProcessor::producesMidi() const
{
    return false;
}

bool MidSideAudioProcessor::isMidiEffect() const
{
    return false;
}

double MidSideAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidSideAudioProcessor::getNumPrograms()
{
    return 1;
}

int MidSideAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidSideAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidSideAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidSideAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidSideAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidSideAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MidSideAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void MidSideAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* channelDataM = buffer.getWritePointer(0);
    auto* channelDataS = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        auto inputM = channelDataM[i];
        auto inputS = channelDataS[i];
        auto leftOut = inputM + inputS / 2.f;
        auto rightOut = inputM - inputS / 2.f;
        channelDataM[i] = leftOut;
        channelDataS[i] = rightOut;
    }
}

//==============================================================================
bool MidSideAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MidSideAudioProcessor::createEditor()
{
    return new MidSideAudioProcessorEditor (*this);
}

//==============================================================================
void MidSideAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidSideAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidSideAudioProcessor();
}
