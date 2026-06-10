#include "EffectsEngine.h"
#include <cmath>

// --- Delay ---
DelayEngine::DelayEngine() {}

void DelayEngine::prepare(double sr, int /*maxBlockSize*/)
{
    sampleRate = sr;
    int maxDelaySamples = static_cast<int>(sr * 2.0);
    bufferL.assign(static_cast<size_t>(maxDelaySamples), 0.0f);
    bufferR.assign(static_cast<size_t>(maxDelaySamples), 0.0f);
    writePos = 0;
    setTime(timeMs);
}

void DelayEngine::process(float& leftSample, float& rightSample)
{
    int bufSize = static_cast<int>(bufferL.size());
    if (bufSize == 0) return;

    int readPos = (writePos - delayLengthSamples + bufSize) % bufSize;

    float delayL = bufferL[static_cast<size_t>(readPos)];
    float delayR = bufferR[static_cast<size_t>(readPos)];

    if (pingPong)
    {
        bufferL[static_cast<size_t>(writePos)] = rightSample + delayR * feedback;
        bufferR[static_cast<size_t>(writePos)] = leftSample + delayL * feedback;
    }
    else
    {
        bufferL[static_cast<size_t>(writePos)] = leftSample + delayL * feedback;
        bufferR[static_cast<size_t>(writePos)] = rightSample + delayR * feedback;
    }

    leftSample  = leftSample * (1.0f - mix) + delayL * mix;
    rightSample = rightSample * (1.0f - mix) + delayR * mix;

    writePos = (writePos + 1) % bufSize;
}

void DelayEngine::reset()
{
    std::fill(bufferL.begin(), bufferL.end(), 0.0f);
    std::fill(bufferR.begin(), bufferR.end(), 0.0f);
    writePos = 0;
}

void DelayEngine::setTime(float ms)
{
    timeMs = ms;
    delayLengthSamples = juce::jmax(1, static_cast<int>(ms * 0.001f * static_cast<float>(sampleRate)));
    if (!bufferL.empty())
        delayLengthSamples = juce::jmin(delayLengthSamples, static_cast<int>(bufferL.size()) - 1);
}

void DelayEngine::setFeedback(float fb) { feedback = juce::jlimit(0.0f, 0.95f, fb); }
void DelayEngine::setMix(float m) { mix = m; }
void DelayEngine::setSync(bool) {}
void DelayEngine::setPingPong(bool pp) { pingPong = pp; }

// --- Reverb ---
ReverbEngine::ReverbEngine()
{
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.3f;
    params.dryLevel = 0.7f;
    params.width = 1.0f;
}

void ReverbEngine::prepare(double sr)
{
    reverb.setSampleRate(sr);
    reverb.setParameters(params);
}

void ReverbEngine::process(float& leftSample, float& rightSample)
{
    float dryL = leftSample, dryR = rightSample;
    reverb.processStereo(&leftSample, &rightSample, 1);
    leftSample  = dryL * (1.0f - mix) + leftSample * mix;
    rightSample = dryR * (1.0f - mix) + rightSample * mix;
}

void ReverbEngine::reset() { reverb.reset(); }

void ReverbEngine::setRoomSize(float size)
{
    params.roomSize = size;
    reverb.setParameters(params);
}

void ReverbEngine::setDamping(float damp)
{
    params.damping = damp;
    reverb.setParameters(params);
}

void ReverbEngine::setMix(float m)
{
    mix = m;
    params.wetLevel = m;
    params.dryLevel = 1.0f - m;
    reverb.setParameters(params);
}

void ReverbEngine::setWidth(float w)
{
    params.width = w;
    reverb.setParameters(params);
}

// --- Compressor ---
CompressorEngine::CompressorEngine() {}

void CompressorEngine::prepare(double sr)
{
    sampleRate = sr;
    envelope = 0.0f;
}

float CompressorEngine::process(float input)
{
    if (!std::isfinite(input)) input = 0.0f;

    float absIn = std::abs(input);
    float inputDb = 20.0f * std::log10(absIn + 1e-6f);
    inputDb = juce::jlimit(-120.0f, 24.0f, inputDb);

    float attackCoeff = std::exp(-1.0f / (juce::jmax(0.1f, attackMs) * 0.001f * static_cast<float>(sampleRate)));
    float releaseCoeff = std::exp(-1.0f / (juce::jmax(1.0f, releaseMs) * 0.001f * static_cast<float>(sampleRate)));

    if (inputDb > envelope)
        envelope = attackCoeff * envelope + (1.0f - attackCoeff) * inputDb;
    else
        envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * inputDb;

    if (!std::isfinite(envelope)) envelope = -120.0f;
    envelope = juce::jlimit(-120.0f, 24.0f, envelope);

    float gainReduction = 0.0f;
    if (envelope > threshold)
        gainReduction = (threshold - envelope) * (1.0f - 1.0f / juce::jmax(1.0f, ratio));

    float gainDb = gainReduction + makeupGain;
    gainDb = juce::jlimit(-60.0f, 24.0f, gainDb);
    float gainLinear = std::pow(10.0f, gainDb / 20.0f);

    float out = input * gainLinear;
    if (!std::isfinite(out)) out = 0.0f;
    return out;
}

void CompressorEngine::reset() { envelope = 0.0f; }

void CompressorEngine::setThreshold(float dB) { threshold = dB; }
void CompressorEngine::setRatio(float r) { ratio = juce::jmax(1.0f, r); }
void CompressorEngine::setAttack(float ms) { attackMs = juce::jmax(0.1f, ms); }
void CompressorEngine::setRelease(float ms) { releaseMs = juce::jmax(1.0f, ms); }
void CompressorEngine::setMakeupGain(float dB) { makeupGain = dB; }

// --- Chorus ---
ChorusEngine::ChorusEngine() {}

void ChorusEngine::prepare(double sr)
{
    sampleRate = sr;
    bufferL.assign(kMaxDelay, 0.0f);
    bufferR.assign(kMaxDelay, 0.0f);
    writePos = 0;
    lfoPhase = 0.0;
}

void ChorusEngine::process(float& leftSample, float& rightSample)
{
    double lfoL = std::sin(lfoPhase * juce::MathConstants<double>::twoPi);
    double lfoR = std::sin((lfoPhase + 0.25) * juce::MathConstants<double>::twoPi);

    float delayMsL = 7.0f + static_cast<float>(lfoL) * depth * 5.0f;
    float delayMsR = 7.0f + static_cast<float>(lfoR) * depth * 5.0f;

    float delaySamplesL = delayMsL * 0.001f * static_cast<float>(sampleRate);
    float delaySamplesR = delayMsR * 0.001f * static_cast<float>(sampleRate);

    auto readInterp = [](const std::vector<float>& buf, int wp, float delaySamples) -> float
    {
        int bufSize = static_cast<int>(buf.size());
        float readPos = static_cast<float>(wp) - delaySamples;
        if (readPos < 0.0f) readPos += static_cast<float>(bufSize);
        int pos0 = static_cast<int>(readPos) % bufSize;
        int pos1 = (pos0 + 1) % bufSize;
        float frac = readPos - std::floor(readPos);
        return buf[static_cast<size_t>(pos0)] * (1.0f - frac) + buf[static_cast<size_t>(pos1)] * frac;
    };

    float wetL = readInterp(bufferL, writePos, delaySamplesL);
    float wetR = readInterp(bufferR, writePos, delaySamplesR);

    bufferL[static_cast<size_t>(writePos)] = leftSample;
    bufferR[static_cast<size_t>(writePos)] = rightSample;
    writePos = (writePos + 1) % kMaxDelay;

    leftSample  = leftSample * (1.0f - mix) + wetL * mix;
    rightSample = rightSample * (1.0f - mix) + wetR * mix;

    lfoPhase += static_cast<double>(rate) / sampleRate;
    if (lfoPhase >= 1.0) lfoPhase -= 1.0;
}

void ChorusEngine::reset()
{
    std::fill(bufferL.begin(), bufferL.end(), 0.0f);
    std::fill(bufferR.begin(), bufferR.end(), 0.0f);
    writePos = 0;
    lfoPhase = 0.0;
}

void ChorusEngine::setRate(float hz) { rate = hz; }
void ChorusEngine::setDepth(float d) { depth = d; }
void ChorusEngine::setMix(float m) { mix = m; }

// --- Phaser ---
PhaserEngine::PhaserEngine() {}

void PhaserEngine::prepare(double sr)
{
    sampleRate = sr;
    reset();
}

void PhaserEngine::process(float& leftSample, float& rightSample)
{
    double lfo = std::sin(lfoPhase * juce::MathConstants<double>::twoPi);
    lfoPhase += static_cast<double>(rate) / sampleRate;
    if (lfoPhase >= 1.0) lfoPhase -= 1.0;

    float minFreq = 200.0f;
    float maxFreq = 4000.0f;
    float modFreq = minFreq + (maxFreq - minFreq) * (0.5f + 0.5f * static_cast<float>(lfo) * depth);
    float coeff = (modFreq * juce::MathConstants<float>::twoPi / static_cast<float>(sampleRate) - 1.0f)
                / (modFreq * juce::MathConstants<float>::twoPi / static_cast<float>(sampleRate) + 1.0f);

    float inL = leftSample + feedbackSampleL * feedback;
    float inR = rightSample + feedbackSampleR * feedback;

    for (int i = 0; i < kNumStages; ++i)
    {
        float tmpL = coeff * inL + allpassL[i];
        allpassL[i] = inL - coeff * tmpL;
        inL = tmpL;

        float tmpR = coeff * inR + allpassR[i];
        allpassR[i] = inR - coeff * tmpR;
        inR = tmpR;
    }

    feedbackSampleL = juce::jlimit(-2.0f, 2.0f, inL);
    feedbackSampleR = juce::jlimit(-2.0f, 2.0f, inR);
    if (!std::isfinite(feedbackSampleL)) feedbackSampleL = 0.0f;
    if (!std::isfinite(feedbackSampleR)) feedbackSampleR = 0.0f;

    leftSample  = leftSample * (1.0f - mix) + inL * mix;
    rightSample = rightSample * (1.0f - mix) + inR * mix;
}

void PhaserEngine::reset()
{
    lfoPhase = 0.0;
    feedbackSampleL = 0.0f;
    feedbackSampleR = 0.0f;
    for (int i = 0; i < kNumStages; ++i)
    {
        allpassL[i] = 0.0f;
        allpassR[i] = 0.0f;
    }
}

void PhaserEngine::setRate(float hz) { rate = hz; }
void PhaserEngine::setDepth(float d) { depth = d; }
void PhaserEngine::setMix(float m) { mix = m; }
void PhaserEngine::setFeedback(float fb) { feedback = juce::jlimit(0.0f, 0.9f, fb); }

// --- EQ ---
EQEngine::EQEngine() { updateCoeffs(); }

void EQEngine::prepare(double sr)
{
    sampleRate = sr;
    reset();
    updateCoeffs();
}

void EQEngine::process(float& sample)
{
    if (!std::isfinite(sample)) { sample = 0.0f; return; }

    float low = lowState + lowCutoff * (sample - lowState);
    lowState = low;

    float high = sample - lowState;
    float tmp = highState + highCutoff * (high - highState);
    highState = tmp;
    float mid = high - highState;
    high = highState;

    if (!std::isfinite(lowState)) lowState = 0.0f;
    if (!std::isfinite(highState)) highState = 0.0f;

    sample = juce::jlimit(-4.0f, 4.0f, low * lowGain + mid * midGain + high * highGain);
}

void EQEngine::reset()
{
    lowState = 0.0f;
    midState = 0.0f;
    highState = 0.0f;
}

void EQEngine::updateCoeffs()
{
    lowCutoff = 1.0f - std::exp(-juce::MathConstants<float>::twoPi * 300.0f / static_cast<float>(sampleRate));
    highCutoff = 1.0f - std::exp(-juce::MathConstants<float>::twoPi * midFreq / static_cast<float>(sampleRate));
}

void EQEngine::setLowGain(float dB) { lowGain = std::pow(10.0f, dB / 20.0f); }
void EQEngine::setMidGain(float dB) { midGain = std::pow(10.0f, dB / 20.0f); }
void EQEngine::setHighGain(float dB) { highGain = std::pow(10.0f, dB / 20.0f); }
void EQEngine::setMidFreq(float hz) { midFreq = hz; updateCoeffs(); }

// --- Ring Modulator ---
RingModEngine::RingModEngine() {}

void RingModEngine::prepare(double sr)
{
    sampleRate = sr;
    phase = 0.0;
}

void RingModEngine::process(float& leftSample, float& rightSample)
{
    float carrier = static_cast<float>(std::sin(phase * juce::MathConstants<double>::twoPi));
    phase += static_cast<double>(frequency) / sampleRate;
    if (phase >= 1.0) phase -= 1.0;

    float modulatedL = leftSample * carrier;
    float modulatedR = rightSample * carrier;

    leftSample  = leftSample * (1.0f - mix) + modulatedL * mix;
    rightSample = rightSample * (1.0f - mix) + modulatedR * mix;
}

void RingModEngine::reset() { phase = 0.0; }
void RingModEngine::setFrequency(float hz) { frequency = hz; }
void RingModEngine::setMix(float m) { mix = m; }

// --- Waveshaper ---
WaveshaperEngine::WaveshaperEngine() {}

void WaveshaperEngine::process(float& sample)
{
    if (amount < 0.001f) return;

    float dry = sample;
    float shaped;

    switch (type)
    {
        case 0: // Tanh
            shaped = std::tanh(sample * (1.0f + amount * 10.0f));
            break;
        case 1: // Sin
            shaped = std::sin(sample * juce::MathConstants<float>::pi * (1.0f + amount * 4.0f));
            break;
        case 2: // Abs
        {
            float gain = 1.0f + amount * 8.0f;
            shaped = std::abs(sample * gain);
            if (shaped > 1.0f) shaped = 2.0f - shaped;
            shaped *= (sample >= 0.0f ? 1.0f : -1.0f);
            break;
        }
        case 3: // Cubic
        {
            float x = sample * (1.0f + amount * 5.0f);
            shaped = x - (x * x * x) / 3.0f;
            break;
        }
        default:
            shaped = sample;
            break;
    }

    sample = juce::jlimit(-2.0f, 2.0f, dry * (1.0f - amount) + shaped * amount);
}

void WaveshaperEngine::setAmount(float a) { amount = a; }
void WaveshaperEngine::setType(int t) { type = t; }
