#pragma once
#include <JuceHeader.h>

enum class OscillatorType
{
    Sine,
    Saw,
    Square,
    Noise,
    Sample
};

class Oscillator
{
public:
    Oscillator();

    void prepare(double sampleRate);
    void setFrequency(float frequency);
    void setType(OscillatorType type);
    void setGain(float gain);
    void setPulseWidth(float pw);
    void setDetune(float cents);

    // Unison: stack of detuned sub-oscillators (supersaw)
    void setUnisonCount(int count);
    void setUnisonDetune(float cents);

    float process();
    void reset();

    OscillatorType getType() const { return type; }

    static constexpr int kMaxUnison = 8;

private:
    float renderShape(double ph);

    OscillatorType type = OscillatorType::Saw;
    double sampleRate = 44100.0;
    float frequency = 440.0f;
    float gain = 1.0f;
    float pulseWidth = 0.5f;
    float detuneAmount = 0.0f;

    int unisonCount = 1;
    float unisonDetune = 0.0f;

    double phases[kMaxUnison] = {};
    double phaseIncrements[kMaxUnison] = {};

    void updateIncrements();

    juce::Random random;
};
