#pragma once
#include <JuceHeader.h>

enum class DistortionType
{
    HardClip,
    SoftClip,
    Foldback,
    BitCrush,
    Rectify,
    Scream
};

class DistortionEngine
{
public:
    DistortionEngine();

    void prepare(double sampleRate);
    float process(float input);
    void reset();

    void setType(DistortionType type);
    void setDrive(float drive);
    void setMix(float mix);
    void setBitDepth(float bits);
    void setDownsampleRate(float rate);

private:
    float hardClip(float input);
    float softClip(float input);
    float foldback(float input);
    float bitCrush(float input);
    float rectify(float input);
    float scream(float input);

    DistortionType type = DistortionType::HardClip;
    float drive = 1.0f;
    float mix = 1.0f;
    float bitDepth = 16.0f;
    float downsampleRate = 1.0f;
    float holdSample = 0.0f;
    float holdCounter = 0.0f;
    double sampleRate = 44100.0;
};
