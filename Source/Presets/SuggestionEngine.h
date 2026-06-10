#pragma once
#include <JuceHeader.h>
#include "PresetManager.h"
#include "MidiPatternBank.h"

struct Suggestion
{
    juce::String label;      // e.g. "Try Bass: Punishment Bass 03"
    int presetIndex  = -1;
    int patternIndex = -1;
    bool isPattern   = false;
};

class SuggestionEngine
{
public:
    SuggestionEngine (PresetManager& pm, MidiPatternBank& pb)
        : presetManager (pm), patternBank (pb)
    {
    }

    /** Return up to 8 suggestions (4 sound presets + 4 patterns) that
        complement the currently selected preset and pattern. */
    std::vector<Suggestion> getSuggestions (int currentPresetIndex,
                                           int currentPatternIndex) const
    {
        std::vector<Suggestion> results;

        // Determine the current pattern's category (empty if nothing selected)
        juce::String patternCategory;
        if (currentPatternIndex >= 0 && currentPatternIndex < patternBank.getNumPatterns())
            patternCategory = patternBank.getPattern (currentPatternIndex).category;

        // --- Sound preset suggestions (up to 4) ---
        auto soundCats = getCompatibleSoundCategories (patternCategory);
        std::vector<int> candidatePresets;

        for (auto& cat : soundCats)
        {
            auto indices = presetManager.getPresetIndicesInCategory (cat);
            for (auto idx : indices)
                candidatePresets.push_back (idx);
        }

        if (! candidatePresets.empty())
        {
            const int count = (int) candidatePresets.size();
            std::set<int> chosen;

            for (int i = 0; i < 4 && (int) chosen.size() < juce::jmin (4, count); ++i)
            {
                int pick = (int) (((unsigned) (currentPresetIndex * 7
                                               + currentPatternIndex * 13
                                               + i * 3)) % (unsigned) count);
                chosen.insert (pick);
            }

            for (auto pick : chosen)
            {
                int idx = candidatePresets[(size_t) pick];
                juce::String cat   = presetManager.getPresetCategory (idx);
                juce::String name  = presetManager.getPresetName (idx);

                Suggestion s;
                s.label       = "Try " + cat + ": " + name;
                s.presetIndex = idx;
                s.isPattern   = false;
                results.push_back (s);
            }
        }

        // --- Pattern suggestions (up to 4) ---
        auto patCats = getCompatiblePatternCategories (patternCategory);
        std::vector<int> candidatePatterns;

        for (auto& cat : patCats)
        {
            auto indices = patternBank.getPatternsInCategory (cat);
            for (auto idx : indices)
                candidatePatterns.push_back (idx);
        }

        if (! candidatePatterns.empty())
        {
            const int count = (int) candidatePatterns.size();
            std::set<int> chosen;

            for (int i = 0; i < 4 && (int) chosen.size() < juce::jmin (4, count); ++i)
            {
                int pick = (int) (((unsigned) (currentPresetIndex * 7
                                               + currentPatternIndex * 13
                                               + i * 3)) % (unsigned) count);
                chosen.insert (pick);
            }

            for (auto pick : chosen)
            {
                int idx = candidatePatterns[(size_t) pick];
                const auto& pat = patternBank.getPattern (idx);

                Suggestion s;
                s.label        = "Add " + pat.category + ": " + pat.name;
                s.patternIndex = idx;
                s.isPattern    = true;
                results.push_back (s);
            }
        }

        return results;
    }

private:
    PresetManager&  presetManager;
    MidiPatternBank& patternBank;

    /** Given the current pattern's category, return the sound-preset
        categories that would complement it. */
    std::vector<juce::String> getCompatibleSoundCategories (const juce::String& patternCategory) const
    {
        if (patternCategory.equalsIgnoreCase ("Bass"))
            return { "Leads", "Stabs", "Pads" };

        if (patternCategory.equalsIgnoreCase ("Melody"))
            return { "Bass", "Pads", "Kicks" };

        if (patternCategory.equalsIgnoreCase ("Keys"))
            return { "Leads", "Bass", "FX" };

        // No pattern selected -- fall back to a broad mix
        return { "Bass", "Leads", "Pads", "Stabs" };
    }

    /** Given the current pattern's category, return the pattern
        categories that would complement it. */
    std::vector<juce::String> getCompatiblePatternCategories (const juce::String& patternCategory) const
    {
        if (patternCategory.equalsIgnoreCase ("Bass"))
            return { "Melody", "Keys" };

        if (patternCategory.equalsIgnoreCase ("Melody"))
            return { "Bass", "Keys" };

        if (patternCategory.equalsIgnoreCase ("Keys"))
            return { "Melody", "Bass" };

        // No pattern selected -- suggest everything
        return { "Bass", "Melody", "Keys" };
    }
};
