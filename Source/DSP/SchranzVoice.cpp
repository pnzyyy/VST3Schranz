#include "SchranzVoice.h"

SchranzVoice::SchranzVoice() {}

bool SchranzVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SchranzSound*>(sound) != nullptr;
}

void SchranzVoice::prepare(double sampleRate, int samplesPerBlock)
{
    osc1.prepare(sampleRate);
    osc2.prepare(sampleRate);
    sampleEngine.prepare(sampleRate, samplesPerBlock);
    distortion.prepare(sampleRate);
    filter.prepare(sampleRate);
}

void SchranzVoice::startNote(int midiNoteNumber, float velocity,
                             juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    float freq = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    osc1.setFrequency(freq);
    osc2.setFrequency(freq);
    osc1.reset();
    osc2.reset();
    sampleEngine.noteOn(midiNoteNumber, velocity);
    filter.triggerEnvelope();

    ampEnvValue = 0.0f;
    ampStage = AmpStage::Attack;
    isActive = true;
    masterGain = velocity * 0.3f;
}

void SchranzVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        ampStage = AmpStage::Release;
        filter.releaseEnvelope();
        sampleEngine.noteOff();
    }
    else
    {
        ampStage = AmpStage::Idle;
        ampEnvValue = 0.0f;
        isActive = false;
        clearCurrentNote();
    }
}

void SchranzVoice::pitchWheelMoved(int) {}
void SchranzVoice::controllerMoved(int, int) {}

void SchranzVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                    int startSample, int numSamples)
{
    if (!isActive)
        return;

    float sr = static_cast<float>(getSampleRate());

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Amp envelope
        float envSpeed = 0.0f;
        switch (ampStage)
        {
            case AmpStage::Attack:
                envSpeed = 1.0f / (ampAttack * sr);
                ampEnvValue += envSpeed;
                if (ampEnvValue >= 1.0f) { ampEnvValue = 1.0f; ampStage = AmpStage::Decay; }
                break;
            case AmpStage::Decay:
                envSpeed = 1.0f / (ampDecay * sr);
                ampEnvValue -= envSpeed;
                if (ampEnvValue <= ampSustain) { ampEnvValue = ampSustain; ampStage = AmpStage::Sustain; }
                break;
            case AmpStage::Sustain:
                break;
            case AmpStage::Release:
                envSpeed = 1.0f / (ampRelease * sr);
                ampEnvValue -= envSpeed;
                if (ampEnvValue <= 0.0f)
                {
                    ampEnvValue = 0.0f;
                    ampStage = AmpStage::Idle;
                    isActive = false;
                    clearCurrentNote();
                    return;
                }
                break;
            case AmpStage::Idle:
                isActive = false;
                clearCurrentNote();
                return;
        }

        float osc1Sample = osc1.process() * osc1Level;
        float osc2Sample = osc2.process() * osc2Level;
        float smpSample = sampleEngine.process() * sampleLevel;

        float mixed = juce::jlimit(-2.0f, 2.0f, osc1Sample + osc2Sample + smpSample);
        float distorted = distortion.process(mixed);
        distorted = juce::jlimit(-2.0f, 2.0f, distorted);
        float filtered = filter.process(distorted);
        float output = juce::jlimit(-1.0f, 1.0f, filtered) * ampEnvValue * masterGain;

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            outputBuffer.addSample(channel, startSample + sample, output);
    }
}
