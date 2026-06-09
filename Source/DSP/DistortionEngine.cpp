#include "DistortionEngine.h"
#include <cmath>

DistortionEngine::DistortionEngine() {}

void DistortionEngine::prepare(double sr)
{
    sampleRate = sr;
    reset();
}

float DistortionEngine::process(float input)
{
    float driven = input * drive;
    float distorted = 0.0f;

    switch (type)
    {
        case DistortionType::HardClip:  distorted = hardClip(driven); break;
        case DistortionType::SoftClip:  distorted = softClip(driven); break;
        case DistortionType::Foldback:  distorted = foldback(driven); break;
        case DistortionType::BitCrush:  distorted = bitCrush(driven); break;
        case DistortionType::Rectify:   distorted = rectify(driven); break;
        case DistortionType::Scream:    distorted = scream(driven); break;
    }

    return input * (1.0f - mix) + distorted * mix;
}

void DistortionEngine::reset()
{
    holdSample = 0.0f;
    holdCounter = 0.0f;
}

void DistortionEngine::setType(DistortionType t) { type = t; }
void DistortionEngine::setDrive(float d) { drive = d; }
void DistortionEngine::setMix(float m) { mix = m; }
void DistortionEngine::setBitDepth(float b) { bitDepth = b; }
void DistortionEngine::setDownsampleRate(float r) { downsampleRate = r; }

float DistortionEngine::hardClip(float input)
{
    return juce::jlimit(-1.0f, 1.0f, input);
}

float DistortionEngine::softClip(float input)
{
    return std::tanh(input);
}

float DistortionEngine::foldback(float input)
{
    while (input > 1.0f || input < -1.0f)
    {
        if (input > 1.0f)
            input = 2.0f - input;
        else if (input < -1.0f)
            input = -2.0f - input;
    }
    return input;
}

float DistortionEngine::bitCrush(float input)
{
    holdCounter += 1.0f;
    if (holdCounter >= downsampleRate)
    {
        holdCounter = 0.0f;
        float levels = std::pow(2.0f, bitDepth);
        holdSample = std::round(input * levels) / levels;
    }
    return holdSample;
}

float DistortionEngine::rectify(float input)
{
    return std::abs(input);
}

float DistortionEngine::scream(float input)
{
    // Asymmetric waveshaping for harsh industrial character
    float shaped = std::tanh(input * 3.0f);
    shaped = hardClip(shaped * 2.0f);
    float folded = foldback(input * 1.5f);
    return shaped * 0.7f + folded * 0.3f;
}
