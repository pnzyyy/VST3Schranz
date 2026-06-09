#include "PresetManager.h"
#include "FactoryPresets.h"

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& a)
    : apvts(a)
{
    initFactoryPresets();
}

void PresetManager::initFactoryPresets()
{
    presets.clear();
    auto factoryPresets = FactoryPresets::createAll();
    for (auto& p : factoryPresets)
        presets.add(p);
}

void PresetManager::loadPreset(int index)
{
    if (index >= 0 && index < presets.size())
    {
        currentIndex = index;
        applyPreset(presets[index].state);
    }
}

void PresetManager::loadPreset(const juce::String& name)
{
    for (int i = 0; i < presets.size(); ++i)
    {
        if (presets[i].name == name)
        {
            loadPreset(i);
            return;
        }
    }
}

void PresetManager::saveUserPreset(const juce::String& name)
{
    PresetData preset;
    preset.name = name;
    preset.state = captureCurrentState();
    presets.add(preset);
    currentIndex = presets.size() - 1;
}

int PresetManager::getNumPresets() const { return presets.size(); }

juce::String PresetManager::getPresetName(int index) const
{
    if (index >= 0 && index < presets.size())
        return presets[index].name;
    return {};
}

juce::StringArray PresetManager::getPresetNames() const
{
    juce::StringArray names;
    for (auto& p : presets)
        names.add(p.name);
    return names;
}

void PresetManager::nextPreset()
{
    if (presets.size() > 0)
        loadPreset((currentIndex + 1) % presets.size());
}

void PresetManager::previousPreset()
{
    if (presets.size() > 0)
        loadPreset((currentIndex - 1 + presets.size()) % presets.size());
}

void PresetManager::applyPreset(const juce::ValueTree& state)
{
    for (int i = 0; i < state.getNumProperties(); ++i)
    {
        auto paramId = state.getPropertyName(i).toString();
        if (auto* param = apvts.getParameter(paramId))
        {
            float value = static_cast<float>(state.getProperty(paramId));
            param->setValueNotifyingHost(param->convertTo0to1(value));
        }
    }
}

juce::ValueTree PresetManager::captureCurrentState()
{
    juce::ValueTree state("Preset");
    auto& params = apvts.processor.getParameters();
    for (auto* param : params)
    {
        if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*>(param))
            state.setProperty(juce::Identifier(rangedParam->getParameterID()),
                              rangedParam->convertFrom0to1(rangedParam->getValue()), nullptr);
    }
    return state;
}

juce::File PresetManager::getPresetDirectory() const
{
    auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                   .getChildFile("SchranzMachine")
                   .getChildFile("Presets");
    dir.createDirectory();
    return dir;
}
