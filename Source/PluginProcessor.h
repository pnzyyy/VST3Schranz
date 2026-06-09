#pragma once
#include <JuceHeader.h>
#include "DSP/SchranzSynth.h"
#include "Presets/PresetManager.h"

class SchranzMachineProcessor : public juce::AudioProcessor
{
public:
    SchranzMachineProcessor();
    ~SchranzMachineProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    PresetManager& getPresetManager() { return presetManager; }
    SchranzSynth& getSchranzSynth() { return schranzSynth; }

    void loadSample(const juce::File& file);
    juce::String getLoadedSampleName() const { return loadedSampleName; }

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState apvts;
    SchranzSynth schranzSynth;
    PresetManager presetManager;
    juce::String loadedSampleName;
    juce::String loadedSamplePath;

    void updateVoiceParameters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SchranzMachineProcessor)
};
