#pragma once
#include <JuceHeader.h>

struct PresetData
{
    juce::String name;
    juce::ValueTree state;
};

class PresetManager
{
public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);

    void loadPreset(int index);
    void loadPreset(const juce::String& name);
    void saveUserPreset(const juce::String& name);

    int getNumPresets() const;
    juce::String getPresetName(int index) const;
    int getCurrentPresetIndex() const { return currentIndex; }
    juce::StringArray getPresetNames() const;

    void nextPreset();
    void previousPreset();

    void initFactoryPresets();

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::Array<PresetData> presets;
    int currentIndex = 0;

    void applyPreset(const juce::ValueTree& state);
    juce::ValueTree captureCurrentState();
    juce::File getPresetDirectory() const;
};
