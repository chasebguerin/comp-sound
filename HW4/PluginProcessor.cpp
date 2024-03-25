/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestingPitchAudioProcessor::TestingPitchAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    // added
    pitchParameter(nullptr),
    gainParameter(nullptr)
#endif
{
    // added
    addParameter(pitchParameter = new juce::AudioParameterFloat("pitch", "Pitch", -12.0f, 12.0f, 0.0f));
    addParameter(gainParameter = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));

}

// added
void TestingPitchAudioProcessor::setPitch(float newPitch)
{
    *pitchParameter = newPitch;
}

void TestingPitchAudioProcessor::setGain(float newGain)
{
    if (gainParameter != nullptr)
    {
        *gainParameter = newGain;
    }
}


TestingPitchAudioProcessor::~TestingPitchAudioProcessor()
{
}

//==============================================================================
const juce::String TestingPitchAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestingPitchAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestingPitchAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestingPitchAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestingPitchAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestingPitchAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestingPitchAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestingPitchAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TestingPitchAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestingPitchAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TestingPitchAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TestingPitchAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestingPitchAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TestingPitchAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

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
    
    // added
    float pitchShiftFactor = powf(2.0, pitchParameter->get() / 12.0);
    float currentGain = gainParameter->get();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // added
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {

            int readIndex = static_cast<int>(sample * pitchShiftFactor);
            if (readIndex < buffer.getNumSamples())
            {
                channelData[sample] = channelData[readIndex];
            }
            else
            {
                channelData[sample] = 0;
            }
            
            channelData[sample] *= currentGain;
        }
        
        
        // ..do something to the data...
    }
}

//==============================================================================
bool TestingPitchAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestingPitchAudioProcessor::createEditor()
{
    return new TestingPitchAudioProcessorEditor (*this);
}

//==============================================================================
void TestingPitchAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestingPitchAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestingPitchAudioProcessor();
}
