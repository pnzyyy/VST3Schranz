#include "Oscillator.h"

Oscillator::Oscillator() {}

void Oscillator::prepare(double sr)
{
    sampleRate = sr;
    reset();
}

void Oscillator::setFrequency(float freq)
{
    frequency = freq;
    float detuned = frequency * std::pow(2.0f, detuneAmount / 1200.0f);
    phaseIncrement = static_cast<double>(detuned) / sampleRate;
}

void Oscillator::setType(OscillatorType t)
{
    type = t;
}

void Oscillator::setGain(float g)
{
    gain = g;
}

void Oscillator::setPulseWidth(float pw)
{
    pulseWidth = juce::jlimit(0.05f, 0.95f, pw);
}

void Oscillator::setDetune(float cents)
{
    detuneAmount = cents;
    setFrequency(frequency);
}

float Oscillator::process()
{
    float sample = 0.0f;

    switch (type)
    {
        case OscillatorType::Sine:   sample = processSine(); break;
        case OscillatorType::Saw:    sample = processSaw(); break;
        case OscillatorType::Square: sample = processSquare(); break;
        case OscillatorType::Noise:  sample = processNoise(); break;
        case OscillatorType::Sample: sample = 0.0f; break;
    }

    phase += phaseIncrement;
    if (phase >= 1.0)
        phase -= 1.0;

    return sample * gain;
}

void Oscillator::reset()
{
    phase = 0.0;
}

float Oscillator::processSine()
{
    return std::sin(static_cast<float>(phase * juce::MathConstants<double>::twoPi));
}

float Oscillator::processSaw()
{
    return static_cast<float>(2.0 * phase - 1.0);
}

float Oscillator::processSquare()
{
    return phase < static_cast<double>(pulseWidth) ? 1.0f : -1.0f;
}

float Oscillator::processNoise()
{
    return random.nextFloat() * 2.0f - 1.0f;
}
