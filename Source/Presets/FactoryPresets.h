#pragma once
#include <JuceHeader.h>
#include "PresetManager.h"

namespace FactoryPresets
{

inline PresetData make(const juce::String& name,
                       int osc1Type, float osc1Gain, float osc1Detune,
                       int osc2Type, float osc2Gain, float osc2Detune,
                       float attack, float decay, float sustain, float release,
                       int distType, float distDrive, float distMix,
                       float crushBits, float crushRate,
                       int filterType, float filterCutoff, float filterRes, float filterEnv,
                       float masterGain)
{
    PresetData p;
    p.name = name;
    p.state = juce::ValueTree("Preset");
    auto& s = p.state;

    s.setProperty("osc1Type", osc1Type, nullptr);
    s.setProperty("osc1Gain", osc1Gain, nullptr);
    s.setProperty("osc1Detune", osc1Detune, nullptr);
    s.setProperty("osc2Type", osc2Type, nullptr);
    s.setProperty("osc2Gain", osc2Gain, nullptr);
    s.setProperty("osc2Detune", osc2Detune, nullptr);

    s.setProperty("ampAttack", attack, nullptr);
    s.setProperty("ampDecay", decay, nullptr);
    s.setProperty("ampSustain", sustain, nullptr);
    s.setProperty("ampRelease", release, nullptr);

    s.setProperty("distType", distType, nullptr);
    s.setProperty("distDrive", distDrive, nullptr);
    s.setProperty("distMix", distMix, nullptr);

    s.setProperty("crushBits", crushBits, nullptr);
    s.setProperty("crushRate", crushRate, nullptr);

    s.setProperty("filterType", filterType, nullptr);
    s.setProperty("filterCutoff", filterCutoff, nullptr);
    s.setProperty("filterResonance", filterRes, nullptr);
    s.setProperty("filterEnvAmount", filterEnv, nullptr);

    s.setProperty("masterGain", masterGain, nullptr);

    return p;
}

inline juce::Array<PresetData> createAll()
{
    juce::Array<PresetData> presets;

    //                          name                  o1T  o1G  o1D  o2T  o2G  o2D    A      D     S     R    dT   dDr  dMx  cB    cR    fT   fCut   fRes  fEnv  mGain
    presets.add(make("Demolition Kick",                0, 0.9f, 0.f,  3, 0.2f, 0.f, 0.001f, 0.15f, 0.0f, 0.05f, 0, 0.9f, 0.9f, 16.f, 1.f,  0, 200.f,  0.7f, 0.5f, 0.8f));
    presets.add(make("Industrial Hammer",              2, 0.8f, 0.f,  3, 0.3f, 0.f, 0.001f, 0.08f, 0.0f, 0.03f, 0, 0.7f, 0.8f,  6.f, 3.f,  0, 800.f,  1.5f, 0.3f, 0.7f));
    presets.add(make("Acid Scream",                    1, 0.8f, 0.f,  1, 0.3f, 7.f, 0.001f, 0.20f, 0.6f, 0.15f, 1, 0.5f, 0.6f, 16.f, 1.f,  0, 800.f,  8.0f, 0.7f, 0.7f));
    presets.add(make("Concrete Texture",               3, 0.6f, 0.f,  1, 0.3f, 0.f, 0.300f, 0.40f, 0.6f, 1.00f, 1, 0.3f, 0.4f,  8.f, 4.f,  0, 3000.f, 1.0f, 0.2f, 0.6f));
    presets.add(make("Razor Lead",                     1, 0.7f, 0.f,  1, 0.6f,15.f, 0.001f, 0.10f, 0.8f, 0.10f, 0, 0.6f, 0.7f, 16.f, 1.f,  0, 5000.f, 2.0f, 0.3f, 0.7f));
    presets.add(make("Bit Rot",                        2, 0.7f, 0.f,  3, 0.2f, 0.f, 0.001f, 0.15f, 0.5f, 0.20f, 0, 0.4f, 0.5f,  4.f, 6.f,  0, 6000.f, 1.0f, 0.0f, 0.6f));
    presets.add(make("Warehouse Stab",                 1, 0.7f, 0.f,  2, 0.5f, 0.f, 0.001f, 0.20f, 0.3f, 0.10f, 1, 0.5f, 0.5f, 16.f, 1.f,  0, 3000.f, 1.5f, 0.4f, 0.7f));
    presets.add(make("Punishment Bass",                0, 0.9f, 0.f,  1, 0.4f, 0.f, 0.005f, 0.15f, 0.7f, 0.15f, 0, 0.8f, 0.7f, 16.f, 1.f,  0, 400.f,  2.0f, 0.2f, 0.8f));
    presets.add(make("Broken Machine",                 2, 0.6f, 0.f,  3, 0.5f, 0.f, 0.001f, 0.10f, 0.4f, 0.08f, 5, 0.6f, 0.7f,  3.f, 8.f,  0, 2000.f, 3.0f, 0.5f, 0.5f));
    presets.add(make("Feedback Loop",                  3, 0.3f, 0.f,  0, 0.2f, 0.f, 0.050f, 0.30f, 0.5f, 0.50f, 1, 0.7f, 0.5f, 16.f, 1.f,  2, 1200.f,15.0f, 0.6f, 0.6f));
    presets.add(make("Piledriver",                     1, 0.7f,-7.f,  1, 0.7f, 7.f, 0.001f, 0.10f, 0.7f, 0.10f, 0, 0.85f,0.8f, 16.f, 1.f,  0, 8000.f, 1.5f, 0.2f, 0.7f));
    presets.add(make("Sample Mangler",                 1, 0.3f, 0.f,  3, 0.2f, 0.f, 0.001f, 0.20f, 0.6f, 0.20f, 5, 0.4f, 0.5f, 10.f, 3.f,  0, 8000.f, 2.0f, 0.3f, 0.6f));

    return presets;
}

} // namespace FactoryPresets
