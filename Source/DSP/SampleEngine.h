#pragma once
#include <JuceHeader.h>

class SampleEngine
{
public:
    SampleEngine();

    void prepare(double sampleRate, int samplesPerBlock);
    bool loadSample(const juce::File& file);
    bool loadSample(const void* data, size_t dataSize);

    void noteOn(int midiNote, float velocity);
    void noteOff();
    float process();
    void reset();

    bool hasSample() const { return sampleBuffer.getNumSamples() > 0; }
    juce::String getSampleName() const { return sampleName; }

    void setRootNote(int note) { rootNote = note; }
    void setLoopEnabled(bool enabled) { loopEnabled = enabled; }
    void setStartPosition(float normalised) { startPos = normalised; }
    void setEndPosition(float normalised) { endPos = normalised; }
    void setGain(float g) { gain = g; }

private:
    juce::AudioBuffer<float> sampleBuffer;
    double sourceSampleRate = 44100.0;
    double currentSampleRate = 44100.0;
    double playbackPosition = 0.0;
    double playbackRate = 1.0;
    bool isPlaying = false;
    int rootNote = 60;
    bool loopEnabled = false;
    float startPos = 0.0f;
    float endPos = 1.0f;
    float gain = 1.0f;
    float velocity = 1.0f;
    juce::String sampleName;

    juce::AudioFormatManager formatManager;
};
