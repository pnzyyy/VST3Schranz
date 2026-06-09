#include "FilterEngine.h"
#include <cmath>

FilterEngine::FilterEngine() {}

void FilterEngine::prepare(double sr)
{
    sampleRate = sr;
    reset();
}

float FilterEngine::process(float input)
{
    // Process envelope
    float envSpeed = 0.0f;
    switch (envStage)
    {
        case EnvStage::Attack:
            envSpeed = 1.0f / (envelopeAttack * static_cast<float>(sampleRate));
            envelopeValue += envSpeed;
            if (envelopeValue >= 1.0f) { envelopeValue = 1.0f; envStage = EnvStage::Decay; }
            break;
        case EnvStage::Decay:
            envSpeed = 1.0f / (envelopeDecay * static_cast<float>(sampleRate));
            envelopeValue -= envSpeed;
            if (envelopeValue <= envelopeSustain) { envelopeValue = envelopeSustain; envStage = EnvStage::Sustain; }
            break;
        case EnvStage::Sustain:
            break;
        case EnvStage::Release:
            envSpeed = 1.0f / (envelopeRelease * static_cast<float>(sampleRate));
            envelopeValue -= envSpeed;
            if (envelopeValue <= 0.0f) { envelopeValue = 0.0f; envStage = EnvStage::Idle; }
            break;
        case EnvStage::Idle:
            break;
    }

    float modulatedCutoff = cutoff + envelopeAmount * envelopeValue * 10000.0f;
    modulatedCutoff = juce::jlimit(20.0f, 20000.0f, modulatedCutoff);

    f = 2.0f * std::sin(juce::MathConstants<float>::pi * modulatedCutoff / static_cast<float>(sampleRate));
    q = 1.0f / resonance;

    hp = input - lp - q * bp;
    bp += f * hp;
    lp += f * bp;

    switch (type)
    {
        case FilterType::LowPass:  return lp;
        case FilterType::HighPass: return hp;
        case FilterType::BandPass: return bp;
        case FilterType::Notch:    return lp + hp;
    }
    return input;
}

void FilterEngine::reset()
{
    lp = bp = hp = 0.0f;
    envelopeValue = 0.0f;
    envStage = EnvStage::Idle;
}

void FilterEngine::setType(FilterType t) { type = t; }
void FilterEngine::setCutoff(float freq) { cutoff = juce::jlimit(20.0f, 20000.0f, freq); }
void FilterEngine::setResonance(float r) { resonance = juce::jlimit(0.1f, 20.0f, r); }
void FilterEngine::setEnvelopeAmount(float a) { envelopeAmount = a; }

void FilterEngine::triggerEnvelope()
{
    envelopeGate = true;
    envStage = EnvStage::Attack;
}

void FilterEngine::releaseEnvelope()
{
    envelopeGate = false;
    envStage = EnvStage::Release;
}
