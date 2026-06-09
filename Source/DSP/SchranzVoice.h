#pragma once
#include <JuceHeader.h>
#include "Oscillator.h"
#include "SampleEngine.h"
#include "DistortionEngine.h"
#include "FilterEngine.h"

struct SchranzSound : public juce::SynthesiserSound
{
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class SchranzVoice : public juce::SynthesiserVoice
{
public:
    SchranzVoice();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample, int numSamples) override;

    void prepare(double sampleRate, int samplesPerBlock);

    Oscillator& getOsc1() { return osc1; }
    Oscillator& getOsc2() { return osc2; }
    SampleEngine& getSampleEngine() { return sampleEngine; }
    DistortionEngine& getDistortion() { return distortion; }
    FilterEngine& getFilter() { return filter; }

    void setOsc1Gain(float g) { osc1Level = g; }
    void setOsc2Gain(float g) { osc2Level = g; }
    void setSampleGain(float g) { sampleLevel = g; }
    void setAmpAttack(float a) { ampAttack = a; }
    void setAmpDecay(float d) { ampDecay = d; }
    void setAmpSustain(float s) { ampSustain = s; }
    void setAmpRelease(float r) { ampRelease = r; }

private:
    Oscillator osc1;
    Oscillator osc2;
    SampleEngine sampleEngine;
    DistortionEngine distortion;
    FilterEngine filter;

    float osc1Level = 0.8f;
    float osc2Level = 0.5f;
    float sampleLevel = 0.7f;

    // Amp envelope
    float ampAttack = 0.001f;
    float ampDecay = 0.1f;
    float ampSustain = 0.8f;
    float ampRelease = 0.05f;
    float ampEnvValue = 0.0f;
    enum class AmpStage { Attack, Decay, Sustain, Release, Idle } ampStage = AmpStage::Idle;

    float masterGain = 0.3f;
    bool isActive = false;
};
