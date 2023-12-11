#pragma once

#include <array>
#include <memory>

#include <JuceHeader.h>
#include "DecibelSlider.h"
#include "SineOscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void resetParameters();

private:
    //==============================================================================
    DecibelSlider decibelSlider;
    juce::Label decibelLabel;
    juce::Slider freqSlider;
    juce::Label freqLabel;
    std::unique_ptr<SineOscillator> oscillator;
    float level = 0.0f;
    // float pi2sr;                /*!< 2*pi/sr  */
    float currentFreq = 440.0;  /*!< initial frequency */
    // float w;                    /*!< frequency in rad/s */
    // std::array<float,2> y0;     /*!< output at n-2 samples */
    // std::array<float,2> y1;     /*!< output at n-1 samples */
    // vars to smoothly change gain
    float currentLevel;
    float targetLevel;
    int samplesToTarget;
    static constexpr auto rampLengthSamples = 128;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
