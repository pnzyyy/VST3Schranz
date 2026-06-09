#pragma once
#include <JuceHeader.h>

enum class FilterType
{
    LowPass,
    HighPass,
    BandPass,
    Notch
};

class FilterEngine
{
public:
    FilterEngine();

    void prepare(double sampleRate);
    float process(float input);
    void reset();

    void setType(FilterType type);
    void setCutoff(float frequency);
    void setResonance(float q);
    void setEnvelopeAmount(float amount);

    void triggerEnvelope();
    void releaseEnvelope();

private:
    void updateCoefficients();

    FilterType type = FilterType::LowPass;
    double sampleRate = 44100.0;
    float cutoff = 1000.0f;
    float resonance = 0.707f;
    float envelopeAmount = 0.0f;

    float envelopeValue = 0.0f;
    float envelopeAttack = 0.001f;
    float envelopeDecay = 0.1f;
    float envelopeSustain = 0.5f;
    float envelopeRelease = 0.2f;
    bool envelopeGate = false;
    enum class EnvStage { Attack, Decay, Sustain, Release, Idle } envStage = EnvStage::Idle;

    // State variable filter coefficients
    float lp = 0.0f, bp = 0.0f, hp = 0.0f;
    float f = 0.0f, q = 0.0f;
};
