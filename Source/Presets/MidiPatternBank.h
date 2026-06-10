#pragma once
#include <JuceHeader.h>

struct MidiPatternNote
{
    int noteNumber;
    int velocity;
    double startBeat;
    double duration;
};

struct MidiPattern
{
    juce::String name;
    juce::String category; // "Melody", "Bass", "Keys"
    int lengthInBeats;
    std::vector<MidiPatternNote> notes;
};

class MidiPatternBank
{
public:
    MidiPatternBank();

    int getNumPatterns() const;
    const MidiPattern& getPattern (int index) const;
    juce::StringArray getPatternNames() const;
    juce::StringArray getCategoryNames() const;
    std::vector<int> getPatternsInCategory (const juce::String& category) const;

    juce::MidiMessageSequence generateMidiSequence (int patternIndex, double bpm) const;

private:
    std::vector<MidiPattern> patterns;
    void initPatterns();

    // Helper to create patterns from chord progressions
    void addChordPattern (const juce::String& name, const juce::String& category,
                          int lengthBeats, const std::vector<std::vector<int>>& chords,
                          const std::vector<double>& chordStarts,
                          const std::vector<double>& chordDurations,
                          int baseVelocity = 100);

    void addMelodyPattern (const juce::String& name, int lengthBeats,
                           const std::vector<int>& notes,
                           const std::vector<double>& starts,
                           const std::vector<double>& durations,
                           const std::vector<int>& velocities);

    void addBassPattern (const juce::String& name, int lengthBeats,
                         const std::vector<int>& notes,
                         const std::vector<double>& starts,
                         const std::vector<double>& durations,
                         const std::vector<int>& velocities);
};
