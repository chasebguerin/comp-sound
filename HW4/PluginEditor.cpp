/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestingPitchAudioProcessorEditor::TestingPitchAudioProcessorEditor (TestingPitchAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // added
    addAndMakeVisible(pitchSlider);
    pitchSlider.setRange(-12.0, 12.0);
    pitchSlider.setValue(0);
    pitchSlider.addListener(this);
    
    
    addAndMakeVisible(gainSlider);
    gainSlider.setRange(0.0, 1.0); // Gain range from 0% to 100%
    gainSlider.setValue(0.5);      // Default value: 50%
    gainSlider.addListener(this);
}

TestingPitchAudioProcessorEditor::~TestingPitchAudioProcessorEditor()
{
}


// added
void TestingPitchAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &pitchSlider)
    {
        audioProcessor.setPitch(pitchSlider.getValue());
    }
    else if (slider == &gainSlider) // Add this condition
    {
        audioProcessor.setGain(gainSlider.getValue());
    }
}

//==============================================================================
void TestingPitchAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TestingPitchAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // added
    pitchSlider.setBounds(getLocalBounds().reduced(40));
    
    gainSlider.setBounds(20, 80, getWidth() - 40, 20);
}
