#pragma once
#include <JuceHeader.h>
#include "SchranzVoice.h"

class SchranzSynth
{
public:
    SchranzSynth();

    void prepare(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void setNumVoices(int numVoices);
    SchranzVoice* getVoice(int index);

    bool loadSampleToAllVoices(const juce::File& file);
    bool loadSampleToAllVoices(const void* data, size_t dataSize);

    juce::Synthesiser& getSynth() { return synth; }

    static constexpr int kDefaultVoices = 8;

private:
    juce::Synthesiser synth;
    int currentNumVoices = 0;
};
