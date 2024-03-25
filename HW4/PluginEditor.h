/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TestingPitchAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    TestingPitchAudioProcessorEditor (TestingPitchAudioProcessor&);
    ~TestingPitchAudioProcessorEditor() override;
    
    //added
    void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // added
    juce::Slider pitchSlider;
    juce::Slider gainSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestingPitchAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestingPitchAudioProcessorEditor)
};
