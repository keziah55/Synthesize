#include <iostream>
#include <cmath>
#include <memory>

#include "MainComponent.h"
#include "SineOscillator.h"

//==============================================================================
MainComponent::MainComponent()
{

    decibelSlider.setRange(-100, -12);
    decibelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    // decibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain ((float) decibelSlider.getValue()); };
    decibelSlider.setValue(juce::Decibels::gainToDecibels (level));
    decibelLabel.setText("Noise Level in dB", juce::dontSendNotification);
    decibelSlider.onValueChange = [this]
    {
        targetLevel = juce::Decibels::decibelsToGain ((float) decibelSlider.getValue());
        samplesToTarget = rampLengthSamples;
    };

    freqSlider.setRange(50, 5000);
    freqSlider.setSkewFactorFromMidPoint(500.0);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    // freqSlider.setValue(currentFreq);
    freqLabel.setText("Frequency", juce::dontSendNotification);


    addAndMakeVisible(decibelSlider);
    addAndMakeVisible(decibelLabel);

    addAndMakeVisible(&freqSlider);
    addAndMakeVisible(&freqLabel);

    //setSize(800, 100);
    //setAudioChannels(0, 2);


    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    oscillator = std::make_unique<SineOscillator>(sampleRate);
    freqSlider.setValue(oscillator->getFrequency());

    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    //pi2sr = 2*M_PIf / static_cast<float>(sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //level = 1.0;

    //auto numSamplesRemaining = bufferToFill.numSamples;
    //auto ramp_offset = 0;

    auto freq = (float) freqSlider.getValue();

    oscillator->generate(bufferToFill, freq);

}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    decibelLabel .setBounds (10, 10, 120, 20);
    decibelSlider.setBounds (130, 10, getWidth() - 140, 20);

    freqLabel.setBounds(10, 50, 90, 20);
    freqSlider.setBounds(100, 50, getWidth() - 110, 20);
}

void MainComponent::resetParameters()
{
    currentLevel = targetLevel;
    samplesToTarget = 0;
}
