#include "SampleEngine.h"

SampleEngine::SampleEngine()
{
    formatManager.registerBasicFormats();
}

void SampleEngine::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
}

bool SampleEngine::loadSample(const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr)
        return false;

    sampleBuffer.setSize(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
    reader->read(&sampleBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
    sourceSampleRate = reader->sampleRate;
    sampleName = file.getFileNameWithoutExtension();
    return true;
}

bool SampleEngine::loadSample(const void* data, size_t dataSize)
{
    auto stream = std::make_unique<juce::MemoryInputStream>(data, dataSize, false);
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(std::move(stream)));
    if (reader == nullptr)
        return false;

    sampleBuffer.setSize(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
    reader->read(&sampleBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
    sourceSampleRate = reader->sampleRate;
    sampleName = "Loaded Sample";
    return true;
}

void SampleEngine::noteOn(int midiNote, float vel)
{
    if (!hasSample())
        return;

    velocity = vel;
    playbackRate = std::pow(2.0, (midiNote - rootNote) / 12.0) * (sourceSampleRate / currentSampleRate);
    playbackPosition = static_cast<double>(startPos) * sampleBuffer.getNumSamples();
    isPlaying = true;
}

void SampleEngine::noteOff()
{
    if (!loopEnabled)
        isPlaying = false;
}

float SampleEngine::process()
{
    if (!isPlaying || !hasSample())
        return 0.0f;

    int totalSamples = sampleBuffer.getNumSamples();
    int endSample = static_cast<int>(endPos * totalSamples);

    int pos0 = static_cast<int>(playbackPosition);
    int pos1 = pos0 + 1;
    float frac = static_cast<float>(playbackPosition - pos0);

    if (pos0 >= endSample || pos0 >= totalSamples)
    {
        if (loopEnabled)
        {
            playbackPosition = static_cast<double>(startPos) * totalSamples;
            pos0 = static_cast<int>(playbackPosition);
            pos1 = pos0 + 1;
            frac = static_cast<float>(playbackPosition - pos0);
        }
        else
        {
            isPlaying = false;
            return 0.0f;
        }
    }

    pos1 = juce::jmin(pos1, totalSamples - 1);

    float sample = 0.0f;
    for (int ch = 0; ch < sampleBuffer.getNumChannels(); ++ch)
    {
        auto* data = sampleBuffer.getReadPointer(ch);
        sample += data[pos0] + frac * (data[pos1] - data[pos0]);
    }
    sample /= static_cast<float>(sampleBuffer.getNumChannels());

    playbackPosition += playbackRate;
    return sample * gain * velocity;
}

void SampleEngine::reset()
{
    playbackPosition = 0.0;
    isPlaying = false;
}
