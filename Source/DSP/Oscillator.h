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

    float process();
    void reset();

    OscillatorType getType() const { return type; }

private:
    float processSine();
    float processSaw();
    float processSquare();
    float processNoise();

    OscillatorType type = OscillatorType::Saw;
    double sampleRate = 44100.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
    float frequency = 440.0f;
    float gain = 1.0f;
    float pulseWidth = 0.5f;
    float detuneAmount = 0.0f;

    juce::Random random;
};
