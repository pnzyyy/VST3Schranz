#pragma once
#include <JuceHeader.h>

class DelayEngine
{
public:
    DelayEngine();

    void prepare(double sampleRate, int maxBlockSize);
    void process(float& leftSample, float& rightSample);
    void reset();

    void setTime(float ms);
    void setFeedback(float fb);
    void setMix(float mix);
    void setSync(bool sync);
    void setPingPong(bool pp);

private:
    double sampleRate = 44100.0;
    int delayLengthSamples = 0;
    int writePos = 0;

    std::vector<float> bufferL, bufferR;
    float timeMs = 250.0f;
    float feedback = 0.4f;
    float mix = 0.3f;
    bool pingPong = false;
};

class ReverbEngine
{
public:
    ReverbEngine();

    void prepare(double sampleRate);
    void process(float& leftSample, float& rightSample);
    void reset();

    void setRoomSize(float size);
    void setDamping(float damp);
    void setMix(float mix);
    void setWidth(float width);

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
    float mix = 0.2f;
};

class CompressorEngine
{
public:
    CompressorEngine();

    void prepare(double sampleRate);
    float process(float input);
    void reset();

    void setThreshold(float dB);
    void setRatio(float ratio);
    void setAttack(float ms);
    void setRelease(float ms);
    void setMakeupGain(float dB);

private:
    double sampleRate = 44100.0;
    float threshold = -10.0f;
    float ratio = 4.0f;
    float attackMs = 5.0f;
    float releaseMs = 50.0f;
    float makeupGain = 0.0f;
    float envelope = 0.0f;
};

class ChorusEngine
{
public:
    ChorusEngine();

    void prepare(double sampleRate);
    void process(float& leftSample, float& rightSample);
    void reset();

    void setRate(float hz);
    void setDepth(float depth);
    void setMix(float mix);

private:
    double sampleRate = 44100.0;
    double lfoPhase = 0.0;
    float rate = 0.5f;
    float depth = 0.5f;
    float mix = 0.3f;

    static constexpr int kMaxDelay = 4096;
    std::vector<float> bufferL, bufferR;
    int writePos = 0;
};
