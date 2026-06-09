#include "SchranzSynth.h"

SchranzSynth::SchranzSynth()
{
    synth.addSound(new SchranzSound());
    setNumVoices(kDefaultVoices);
}

void SchranzSynth::prepare(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SchranzVoice*>(synth.getVoice(i)))
            voice->prepare(sampleRate, samplesPerBlock);
    }
}

void SchranzSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void SchranzSynth::setNumVoices(int numVoices)
{
    synth.clearVoices();
    for (int i = 0; i < numVoices; ++i)
        synth.addVoice(new SchranzVoice());
    currentNumVoices = numVoices;
}

SchranzVoice* SchranzSynth::getVoice(int index)
{
    return dynamic_cast<SchranzVoice*>(synth.getVoice(index));
}

bool SchranzSynth::loadSampleToAllVoices(const juce::File& file)
{
    bool success = false;
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SchranzVoice*>(synth.getVoice(i)))
            success = voice->getSampleEngine().loadSample(file);
    }
    return success;
}

bool SchranzSynth::loadSampleToAllVoices(const void* data, size_t dataSize)
{
    bool success = false;
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SchranzVoice*>(synth.getVoice(i)))
            success = voice->getSampleEngine().loadSample(data, dataSize);
    }
    return success;
}
