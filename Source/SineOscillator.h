#pragma once

#include <cmath>
#include <JuceHeader.h>

class SineOscillator {
public:
    SineOscillator(double sampleRate)
    {
        pi2sr = 2*M_PIf / static_cast<float>(sampleRate);
    }

    void setFrequency(float freq)
    {
        currentFreq = freq;
        w = freq * pi2sr;
    }

    float getFrequency()
    {
        return currentFreq;
    }

    void generate(const juce::AudioSourceChannelInfo& bufferToFill, float freq)
    {
        // fill bufferToFill->buffer with sine samples
        // note that all cahnnels in buffer will be given the same values

        float level {0.5};
        w = freq * pi2sr;
        float cos2w {2 * cosf(w)};
        int offset = 0;
        float value;

        if (freq != currentFreq) {

            // first two samples when frequency changed
            history[0] = 0.0;
            history[1] = sinf(w);

            // write to first two samples to buffer
            for (auto channel = 0; channel < y0.size(); channel++) {
                auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
                *buffer++ = history[0] * level;
                *buffer++ = history[1] * level;
            }
            offset = 2;
            currentFreq = freq;
        }

        for (auto sample = offset; sample < bufferToFill.numSamples; sample++) {
            // next value
            value = (cos2w * history[1]) - history[0];

            // write to current sample for all channels
            for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++) {
                bufferToFill.buffer->setSample (channel, sample, value * level); // * currentLevel);
            }

            // update history
            history[0] = history[1];
            history[1] = value;

        }
    }

private:
    float pi2sr;                 /*!< 2*pi/sr  */
    float currentFreq {440.0};   /*!< initial frequency */
    float w;                     /*!< frequency in rad/s */
    std::array<float,2> history; /*!< Output at n-2, n-1 sampes */
};
