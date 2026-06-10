#include "Oscillator.h"

Oscillator::Oscillator() {}

void Oscillator::prepare(double sr)
{
    sampleRate = sr;
    reset();
    updateIncrements();
}

void Oscillator::updateIncrements()
{
    // Centre detune (single global detune knob)
    float baseFreq = frequency * std::pow(2.0f, detuneAmount / 1200.0f);

    if (unisonCount <= 1)
    {
        phaseIncrements[0] = static_cast<double>(baseFreq) / sampleRate;
        return;
    }

    // Spread unison voices symmetrically around the base frequency
    for (int i = 0; i < unisonCount && i < kMaxUnison; ++i)
    {
        // -1..+1 across the stack
        float spread = (static_cast<float>(i) / static_cast<float>(unisonCount - 1)) * 2.0f - 1.0f;
        float cents = spread * unisonDetune;
        float f = baseFreq * std::pow(2.0f, cents / 1200.0f);
        phaseIncrements[i] = static_cast<double>(f) / sampleRate;
    }
}

void Oscillator::setFrequency(float freq)
{
    frequency = freq;
    updateIncrements();
}

void Oscillator::setType(OscillatorType t) { type = t; }
void Oscillator::setGain(float g) { gain = g; }

void Oscillator::setPulseWidth(float pw)
{
    pulseWidth = juce::jlimit(0.05f, 0.95f, pw);
}

void Oscillator::setDetune(float cents)
{
    detuneAmount = cents;
    updateIncrements();
}

void Oscillator::setUnisonCount(int count)
{
    int c = juce::jlimit(1, kMaxUnison, count);
    if (c != unisonCount)
    {
        unisonCount = c;
        updateIncrements();
    }
}

void Oscillator::setUnisonDetune(float cents)
{
    unisonDetune = juce::jmax(0.0f, cents);
    updateIncrements();
}

float Oscillator::renderShape(double ph)
{
    switch (type)
    {
        case OscillatorType::Sine:   return std::sin(static_cast<float>(ph * juce::MathConstants<double>::twoPi));
        case OscillatorType::Saw:    return static_cast<float>(2.0 * ph - 1.0);
        case OscillatorType::Square: return ph < static_cast<double>(pulseWidth) ? 1.0f : -1.0f;
        case OscillatorType::Noise:  return random.nextFloat() * 2.0f - 1.0f;
        case OscillatorType::Sample: return 0.0f;
    }
    return 0.0f;
}

float Oscillator::process()
{
    if (type == OscillatorType::Sample)
        return 0.0f;

    // Noise is not pitched; a single generator is enough.
    if (type == OscillatorType::Noise)
        return (random.nextFloat() * 2.0f - 1.0f) * gain;

    int n = juce::jlimit(1, kMaxUnison, unisonCount);
    float sum = 0.0f;

    for (int i = 0; i < n; ++i)
    {
        sum += renderShape(phases[i]);
        phases[i] += phaseIncrements[i];
        if (phases[i] >= 1.0) phases[i] -= 1.0;
    }

    // Equal-power-ish normalisation so unison doesn't get much louder
    float norm = (n > 1) ? 1.0f / std::sqrt(static_cast<float>(n)) : 1.0f;
    return sum * norm * gain;
}

void Oscillator::reset()
{
    for (int i = 0; i < kMaxUnison; ++i)
    {
        // Slightly randomised start phases so unison voices don't all align
        phases[i] = (unisonCount > 1) ? static_cast<double>(i) / static_cast<double>(kMaxUnison) : 0.0;
    }
}
