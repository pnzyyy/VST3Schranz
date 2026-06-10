#include "MidiPatternBank.h"

//==============================================================================
MidiPatternBank::MidiPatternBank()
{
    initPatterns();
}

int MidiPatternBank::getNumPatterns() const
{
    return static_cast<int> (patterns.size());
}

const MidiPattern& MidiPatternBank::getPattern (int index) const
{
    jassert (index >= 0 && index < static_cast<int> (patterns.size()));
    return patterns[static_cast<size_t> (index)];
}

juce::StringArray MidiPatternBank::getPatternNames() const
{
    juce::StringArray names;
    for (auto& p : patterns)
        names.add (p.name);
    return names;
}

juce::StringArray MidiPatternBank::getCategoryNames() const
{
    return { "Melody", "Bass", "Keys" };
}

std::vector<int> MidiPatternBank::getPatternsInCategory (const juce::String& category) const
{
    std::vector<int> result;
    for (int i = 0; i < static_cast<int> (patterns.size()); ++i)
        if (patterns[static_cast<size_t> (i)].category == category)
            result.push_back (i);
    return result;
}

juce::MidiMessageSequence MidiPatternBank::generateMidiSequence (int patternIndex, double bpm) const
{
    juce::MidiMessageSequence seq;

    if (patternIndex < 0 || patternIndex >= static_cast<int> (patterns.size()))
        return seq;

    const auto& pattern = patterns[static_cast<size_t> (patternIndex)];
    const double secondsPerBeat = 60.0 / bpm;

    for (auto& note : pattern.notes)
    {
        double startTime = note.startBeat * secondsPerBeat;
        double endTime = (note.startBeat + note.duration) * secondsPerBeat;

        auto noteOn = juce::MidiMessage::noteOn (1, note.noteNumber, (juce::uint8) note.velocity);
        noteOn.setTimeStamp (startTime);

        auto noteOff = juce::MidiMessage::noteOff (1, note.noteNumber);
        noteOff.setTimeStamp (endTime);

        seq.addEvent (noteOn);
        seq.addEvent (noteOff);
    }

    seq.updateMatchedPairs();
    return seq;
}

//==============================================================================
void MidiPatternBank::addChordPattern (const juce::String& name, const juce::String& category,
                                       int lengthBeats, const std::vector<std::vector<int>>& chords,
                                       const std::vector<double>& chordStarts,
                                       const std::vector<double>& chordDurations,
                                       int baseVelocity)
{
    MidiPattern p;
    p.name = name;
    p.category = category;
    p.lengthInBeats = lengthBeats;

    for (size_t i = 0; i < chords.size(); ++i)
    {
        double start = chordStarts[i];
        double dur = chordDurations[i];
        for (int noteNum : chords[i])
        {
            p.notes.push_back ({ noteNum, baseVelocity, start, dur });
        }
    }
    patterns.push_back (std::move (p));
}

void MidiPatternBank::addMelodyPattern (const juce::String& name, int lengthBeats,
                                        const std::vector<int>& notes,
                                        const std::vector<double>& starts,
                                        const std::vector<double>& durations,
                                        const std::vector<int>& velocities)
{
    MidiPattern p;
    p.name = name;
    p.category = "Melody";
    p.lengthInBeats = lengthBeats;

    for (size_t i = 0; i < notes.size(); ++i)
    {
        p.notes.push_back ({ notes[i], velocities[i], starts[i], durations[i] });
    }
    patterns.push_back (std::move (p));
}

void MidiPatternBank::addBassPattern (const juce::String& name, int lengthBeats,
                                      const std::vector<int>& notes,
                                      const std::vector<double>& starts,
                                      const std::vector<double>& durations,
                                      const std::vector<int>& velocities)
{
    MidiPattern p;
    p.name = name;
    p.category = "Bass";
    p.lengthInBeats = lengthBeats;

    for (size_t i = 0; i < notes.size(); ++i)
    {
        p.notes.push_back ({ notes[i], velocities[i], starts[i], durations[i] });
    }
    patterns.push_back (std::move (p));
}

//==============================================================================
void MidiPatternBank::initPatterns()
{
    // =========================================================================
    // MELODY PATTERNS (38)
    // =========================================================================

    // --- Arpeggiated patterns ---

    // 1. Dark Tunnel Arp - Am arpeggio ascending
    addMelodyPattern ("Dark Tunnel Arp", 4,
        { 69, 72, 76, 69, 72, 76, 81, 76 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 100, 90, 110, 100, 90, 110, 120, 100 });

    // 2. Warehouse Arp Down - Cm descending arpeggio
    addMelodyPattern ("Warehouse Arp Down", 4,
        { 84, 79, 75, 72, 84, 79, 75, 72 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 100, 90, 80, 110, 100, 90, 80 });

    // 3. Acid Cascade - Em arpeggio with wider intervals
    addMelodyPattern ("Acid Cascade", 4,
        { 64, 71, 76, 83, 76, 71, 64, 71 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 100, 105, 110, 120, 110, 105, 100, 105 });

    // 4. Neon Spiral - Dm arpeggio with octave jumps
    addMelodyPattern ("Neon Spiral", 4,
        { 62, 74, 65, 77, 69, 81, 65, 77 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 95, 110, 95, 110, 95, 110, 95 });

    // 5. Broken Mirror Arp - Gm arpeggio with random feel
    addMelodyPattern ("Broken Mirror Arp", 4,
        { 67, 74, 70, 79, 67, 82, 74, 70 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35 },
        { 100, 110, 90, 120, 100, 115, 105, 90 });

    // 6. Rising Tension Arp - chromatic ascending
    addMelodyPattern ("Rising Tension Arp", 8,
        { 60, 63, 67, 70, 72, 75, 79, 82, 84, 82, 79, 75, 72, 70, 67, 63 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 90, 95, 100, 105, 110, 115, 120, 125, 127, 120, 115, 110, 105, 100, 95, 90 });

    // --- Staccato rhythmic patterns ---

    // 7. Industrial Stab 01 - 16th note staccato Am
    addMelodyPattern ("Industrial Stab 01", 4,
        { 69, 69, 72, 69, 69, 72, 76, 72, 69, 69, 72, 69, 76, 72, 69, 72 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 },
        { 120, 80, 110, 80, 120, 80, 110, 80, 120, 80, 110, 80, 120, 100, 90, 80 });

    // 8. Machine Gun Melody - rapid 16th note pattern Cm
    addMelodyPattern ("Machine Gun Melody", 4,
        { 72, 72, 75, 72, 72, 75, 79, 75, 72, 72, 75, 72, 79, 75, 72, 75 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12 },
        { 127, 90, 120, 90, 127, 90, 120, 90, 127, 90, 120, 90, 127, 110, 100, 90 });

    // 9. Piston Rhythm - staccato Em pattern
    addMelodyPattern ("Piston Rhythm", 4,
        { 76, 76, 79, 76, 83, 79, 76, 79 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 120, 100, 120, 100, 127, 110, 120, 100 });

    // 10. Steel Pulse Stab - quick staccato Dm
    addMelodyPattern ("Steel Pulse Stab", 4,
        { 74, 74, 77, 74, 74, 77, 81, 77, 74, 74, 77, 74, 81, 77, 74, 77 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08 },
        { 110, 80, 100, 80, 110, 80, 100, 80, 110, 80, 100, 80, 110, 100, 90, 80 });

    // --- Syncopated melodies ---

    // 11. Syncopated Warehouse 01 - offbeat accents Am
    addMelodyPattern ("Syncopated Warehouse 01", 4,
        { 69, 72, 76, 72, 69, 76, 72, 69 },
        { 0.25, 0.75, 1.0, 1.75, 2.25, 2.75, 3.0, 3.75 },
        { 0.3, 0.2, 0.5, 0.3, 0.3, 0.2, 0.5, 0.2 },
        { 100, 110, 120, 100, 100, 110, 120, 90 });

    // 12. Off-Grid Sequence - syncopated Cm
    addMelodyPattern ("Off-Grid Sequence", 4,
        { 72, 75, 79, 75, 72, 79, 75, 72 },
        { 0.25, 0.5, 1.25, 1.75, 2.25, 2.5, 3.25, 3.75 },
        { 0.2, 0.5, 0.3, 0.3, 0.2, 0.5, 0.3, 0.2 },
        { 90, 110, 120, 100, 90, 110, 120, 90 });

    // 13. Displaced Rhythm - heavy syncopation Gm
    addMelodyPattern ("Displaced Rhythm", 8,
        { 67, 70, 74, 79, 74, 70, 67, 70, 74, 79, 82, 79, 74, 70, 67, 70 },
        { 0.25, 0.75, 1.25, 1.5, 2.25, 2.75, 3.25, 3.5, 4.25, 4.75, 5.25, 5.5, 6.25, 6.75, 7.25, 7.5 },
        { 0.3, 0.3, 0.2, 0.5, 0.3, 0.3, 0.2, 0.5, 0.3, 0.3, 0.2, 0.5, 0.3, 0.3, 0.2, 0.3 },
        { 100, 110, 90, 120, 100, 110, 90, 120, 100, 110, 90, 120, 100, 110, 90, 100 });

    // 14. Ghost Note Lead - subtle syncopation Em
    addMelodyPattern ("Ghost Note Lead", 4,
        { 76, 79, 83, 79, 76, 83, 79, 76 },
        { 0.0, 0.75, 1.0, 1.75, 2.0, 2.75, 3.0, 3.5 },
        { 0.5, 0.2, 0.5, 0.2, 0.5, 0.2, 0.3, 0.3 },
        { 120, 70, 120, 70, 120, 70, 110, 80 });

    // --- Pentatonic phrases ---

    // 15. Pentatonic Grind 01 - Am pentatonic
    addMelodyPattern ("Pentatonic Grind 01", 4,
        { 69, 72, 74, 76, 81, 76, 74, 72 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 100, 105, 110, 115, 120, 115, 110, 105 });

    // 16. Pentatonic Ripper - Em pentatonic fast
    addMelodyPattern ("Pentatonic Ripper", 4,
        { 64, 67, 69, 71, 76, 71, 69, 67, 64, 67, 69, 71, 76, 71, 69, 67 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 110, 100, 110, 100, 120, 100, 110, 100, 110, 100, 110, 100, 120, 100, 110, 100 });

    // 17. Minor Penta Hook - Dm pentatonic hook
    addMelodyPattern ("Minor Penta Hook", 4,
        { 74, 77, 79, 81, 84, 81, 79, 77 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 110, 100, 115, 105, 120, 105, 115, 100 });

    // 18. Gritty Penta Run - Gm pentatonic
    addMelodyPattern ("Gritty Penta Run", 8,
        { 67, 70, 72, 74, 79, 74, 72, 70, 79, 74, 72, 70, 67, 70, 72, 74 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 100, 105, 110, 115, 120, 115, 110, 105, 120, 115, 110, 105, 100, 105, 110, 115 });

    // --- Dark chromatic runs ---

    // 19. Chromatic Descent - dark chromatic downward
    addMelodyPattern ("Chromatic Descent", 4,
        { 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 80, 85, 90, 95, 100, 105, 110, 115, 120, 120, 115, 110, 105, 100, 95, 90 });

    // 20. Chromatic Ascent - dark chromatic upward
    addMelodyPattern ("Chromatic Ascent", 4,
        { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 90, 95, 100, 105, 110, 115, 120, 125, 127, 125, 120, 115, 110, 105, 100, 95 });

    // 21. Dissonance Crawl - chromatic cluster movement
    addMelodyPattern ("Dissonance Crawl", 4,
        { 69, 70, 72, 73, 69, 70, 72, 73, 75, 76, 72, 73, 69, 70, 72, 69 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 100, 90, 110, 90, 100, 90, 110, 90, 120, 90, 110, 90, 100, 90, 110, 100 });

    // 22. Razor Edge Run - sharp chromatic pattern
    addMelodyPattern ("Razor Edge Run", 4,
        { 72, 73, 75, 72, 73, 75, 76, 78, 75, 76, 78, 79, 78, 76, 75, 72 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 110, 100, 120, 100, 110, 100, 120, 100, 110, 100, 120, 100, 127, 110, 100, 90 });

    // --- Driving 8th note patterns ---

    // 23. Driving Force 01 - Am 8th notes
    addMelodyPattern ("Driving Force 01", 4,
        { 69, 72, 69, 76, 69, 72, 69, 76 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 120, 100, 120, 110, 120, 100, 120, 110 });

    // 24. Relentless Drive - Cm driving melody
    addMelodyPattern ("Relentless Drive", 4,
        { 72, 75, 72, 79, 72, 75, 72, 79 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 120, 100, 120, 110, 120, 100, 120, 110 });

    // 25. Motorik Lead - repetitive driving Em
    addMelodyPattern ("Motorik Lead", 8,
        { 76, 79, 76, 83, 76, 79, 76, 83, 76, 79, 76, 83, 76, 79, 83, 76 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 120, 100, 120, 110, 120, 100, 120, 110, 120, 100, 120, 110, 120, 100, 120, 110 });

    // 26. Highway Melody - Dm driving 8ths
    addMelodyPattern ("Highway Melody", 4,
        { 74, 77, 74, 81, 74, 77, 74, 81 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 95, 110, 105, 110, 95, 110, 105 });

    // --- Offbeat patterns ---

    // 27. Offbeat Puncher - Am offbeat hits
    addMelodyPattern ("Offbeat Puncher", 4,
        { 69, 72, 76, 69, 72, 76, 69, 72 },
        { 0.5, 1.0, 1.5, 2.5, 3.0, 3.5, 0.25, 2.25 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 110, 120, 110, 110, 120, 110, 90, 90 });

    // 28. Backbeat Grinder - offbeat Em pattern
    addMelodyPattern ("Backbeat Grinder", 4,
        { 76, 79, 83, 76, 79, 83, 76, 79 },
        { 0.5, 1.5, 2.5, 3.5, 0.25, 1.25, 2.25, 3.25 },
        { 0.2, 0.2, 0.2, 0.2, 0.15, 0.15, 0.15, 0.15 },
        { 120, 120, 120, 120, 80, 80, 80, 80 });

    // 29. Upbeat Surge - Gm offbeat melody
    addMelodyPattern ("Upbeat Surge", 4,
        { 79, 82, 79, 74, 79, 82, 79, 74 },
        { 0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 100, 110, 100, 90, 100, 110, 100, 90 });

    // --- Triplet patterns ---

    // 30. Triplet Assault 01 - Am triplets
    addMelodyPattern ("Triplet Assault 01", 4,
        { 69, 72, 76, 69, 72, 76, 69, 72, 76, 69, 72, 76 },
        { 0.0, 0.333, 0.667, 1.0, 1.333, 1.667, 2.0, 2.333, 2.667, 3.0, 3.333, 3.667 },
        { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25 },
        { 120, 100, 110, 120, 100, 110, 120, 100, 110, 120, 100, 110 });

    // 31. Triplet Storm - Cm triplets descending
    addMelodyPattern ("Triplet Storm", 4,
        { 84, 79, 75, 79, 75, 72, 75, 72, 67, 72, 67, 63 },
        { 0.0, 0.333, 0.667, 1.0, 1.333, 1.667, 2.0, 2.333, 2.667, 3.0, 3.333, 3.667 },
        { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25 },
        { 110, 105, 100, 110, 105, 100, 110, 105, 100, 110, 105, 100 });

    // 32. Warehouse Melody 01 - Dark Am melody with rests
    addMelodyPattern ("Warehouse Melody 01", 8,
        { 69, 72, 76, 81, 76, 72, 69, 67, 69, 72, 76, 81 },
        { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 6.5, 7.0, 7.25, 7.5, 7.75 },
        { 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.4, 0.4, 0.2, 0.2, 0.2, 0.2 },
        { 100, 110, 120, 127, 120, 110, 100, 90, 110, 115, 120, 127 });

    // 33. Concrete Jungle Lead - Em with tension
    addMelodyPattern ("Concrete Jungle Lead", 4,
        { 64, 67, 71, 76, 75, 71, 67, 64 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 100, 110, 120, 127, 120, 110, 100, 90 });

    // 34. Hydraulic Press - heavy accented Dm
    addMelodyPattern ("Hydraulic Press", 4,
        { 62, 65, 69, 62, 65, 69, 74, 69 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 127, 100, 110, 127, 100, 110, 120, 100 });

    // 35. Voltage Spike - sharp Am melody with big leaps
    addMelodyPattern ("Voltage Spike", 4,
        { 69, 81, 72, 84, 69, 81, 72, 84 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 120, 110, 120, 110, 120, 110, 120, 110 });

    // 36. Toxic Melody - Cm with b5 color
    addMelodyPattern ("Toxic Melody", 4,
        { 72, 75, 78, 79, 78, 75, 72, 75 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 100, 120, 115, 120, 100, 110, 100 });

    // 37. Schranz Lead 01 - aggressive repeated note melody
    addMelodyPattern ("Schranz Lead 01", 4,
        { 69, 69, 69, 72, 69, 69, 69, 76, 69, 69, 69, 72, 69, 69, 76, 72 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 120, 100, 100, 127, 120, 100, 100, 127, 120, 100, 100, 127, 120, 100, 127, 110 });

    // 38. Bunker Sequence - minimal Am sequence
    addMelodyPattern ("Bunker Sequence", 8,
        { 69, 67, 69, 72, 69, 67, 69, 76, 69, 67, 69, 72, 76, 72, 69, 67 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 100, 90, 100, 110, 100, 90, 100, 120, 100, 90, 100, 110, 120, 110, 100, 90 });

    // =========================================================================
    // BASS PATTERNS (38)
    // =========================================================================

    // --- Root note pumping (4-on-the-floor) ---

    // 1. Pumping Root A - classic 4otf bass on A
    addBassPattern ("Pumping Root A", 4,
        { 45, 45, 45, 45 },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 },
        { 120, 120, 120, 120 });

    // 2. Pumping Root C - 4otf on C
    addBassPattern ("Pumping Root C", 4,
        { 48, 48, 48, 48 },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 },
        { 120, 120, 120, 120 });

    // 3. Pumping Root D - 4otf on D
    addBassPattern ("Pumping Root D", 4,
        { 50, 50, 50, 50 },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 },
        { 127, 110, 120, 110 });

    // 4. Pumping Root E - 4otf on E
    addBassPattern ("Pumping Root E", 4,
        { 52, 52, 52, 52 },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 },
        { 127, 110, 120, 110 });

    // 5. Pumping Root G - 4otf on G
    addBassPattern ("Pumping Root G", 4,
        { 43, 43, 43, 43 },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 },
        { 120, 120, 120, 120 });

    // 6. Double Pump A - 8th note pumping
    addBassPattern ("Double Pump A", 4,
        { 45, 45, 45, 45, 45, 45, 45, 45 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 120, 90, 120, 90, 120, 90, 120, 90 });

    // --- Octave jump patterns ---

    // 7. Driving Octave A - octave jumps on A
    addBassPattern ("Driving Octave A", 4,
        { 45, 57, 45, 57, 45, 57, 45, 57 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 120, 100, 120, 100, 120, 100, 120, 100 });

    // 8. Driving Octave C - octave jumps on C
    addBassPattern ("Driving Octave C", 4,
        { 36, 48, 36, 48, 36, 48, 36, 48 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 120, 100, 120, 100, 120, 100, 120, 100 });

    // 9. Octave Smash D - aggressive octave D
    addBassPattern ("Octave Smash D", 4,
        { 38, 50, 38, 50, 38, 50, 38, 50 },
        { 0.0, 0.25, 1.0, 1.25, 2.0, 2.25, 3.0, 3.25 },
        { 0.2, 0.5, 0.2, 0.5, 0.2, 0.5, 0.2, 0.5 },
        { 127, 110, 127, 110, 127, 110, 127, 110 });

    // 10. Power Octave G - heavy G octaves
    addBassPattern ("Power Octave G", 4,
        { 43, 55, 43, 55, 43, 55, 43, 55 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 127, 100, 120, 100, 127, 100, 120, 100 });

    // --- Walking bass lines ---

    // 11. Walking Dark Am - Am walking bass
    addBassPattern ("Walking Dark Am", 4,
        { 45, 48, 52, 55, 57, 55, 52, 48 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 110, 100, 110, 100, 110, 100, 110, 100 });

    // 12. Walking Cm Groove - Cm walking bass
    addBassPattern ("Walking Cm Groove", 4,
        { 48, 51, 55, 58, 60, 58, 55, 51 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 110, 100, 110, 100, 110, 100, 110, 100 });

    // 13. Prowling Bass Em - Em walking
    addBassPattern ("Prowling Bass Em", 8,
        { 40, 43, 47, 52, 47, 43, 40, 43, 47, 52, 55, 52, 47, 43, 40, 43 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 100, 105, 110, 105, 100, 110, 100, 105, 110, 115, 110, 105, 100, 110, 100 });

    // 14. Creeping Walk Dm - Dm chromatic walk
    addBassPattern ("Creeping Walk Dm", 4,
        { 50, 51, 52, 53, 50, 51, 52, 53 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4 },
        { 110, 100, 110, 105, 110, 100, 110, 105 });

    // --- Synth bass stabs ---

    // 15. Synth Stab Am - short stab pattern
    addBassPattern ("Synth Stab Am", 4,
        { 45, 45, 48, 45, 45, 48, 52, 48 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 127, 100, 120, 100, 127, 100, 120, 100 });

    // 16. Knife Stab Bass - aggressive stab Cm
    addBassPattern ("Knife Stab Bass", 4,
        { 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 55, 48, 55, 48 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 },
        { 120, 80, 100, 80, 120, 80, 100, 80, 120, 80, 100, 80, 127, 100, 120, 90 });

    // 17. Stab City - rhythmic stab Em
    addBassPattern ("Stab City", 4,
        { 52, 52, 52, 52, 52, 52, 59, 52 },
        { 0.0, 0.75, 1.0, 1.75, 2.0, 2.75, 3.0, 3.5 },
        { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 },
        { 127, 110, 127, 110, 127, 110, 120, 110 });

    // --- Sub bass drones ---

    // 18. Subterranean Pulse A - low sub drone on A
    addBassPattern ("Subterranean Pulse A", 8,
        { 45, 45, 45, 45 },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 },
        { 100, 100, 100, 100 });

    // 19. Deep Drone C - sub drone on C
    addBassPattern ("Deep Drone C", 8,
        { 36, 36, 36, 36 },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 },
        { 100, 90, 100, 90 });

    // 20. Rumble Drone D - D sub with slight movement
    addBassPattern ("Rumble Drone D", 8,
        { 38, 38, 40, 38 },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 },
        { 100, 95, 105, 100 });

    // 21. Abyss Tone E - low E drone
    addBassPattern ("Abyss Tone E", 8,
        { 40, 40, 40, 40 },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.9, 1.9, 1.9, 1.9 },
        { 90, 90, 90, 90 });

    // --- Acid bass lines ---

    // 22. Acid Grinder 01 - acid bass in Am
    addBassPattern ("Acid Grinder 01", 4,
        { 45, 48, 52, 57, 45, 48, 52, 57, 45, 48, 52, 57, 45, 48, 52, 57 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 120, 100, 110, 90, 120, 100, 110, 90, 120, 100, 110, 90, 120, 100, 110, 90 });

    // 23. Acid Squelch Cm - squelchy Cm pattern
    addBassPattern ("Acid Squelch Cm", 4,
        { 48, 51, 48, 55, 48, 51, 48, 55, 48, 51, 48, 55, 60, 55, 51, 48 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 127, 90, 110, 80, 127, 90, 110, 80, 127, 90, 110, 80, 127, 110, 100, 90 });

    // 24. Acid Burn Em - aggressive acid Em
    addBassPattern ("Acid Burn Em", 4,
        { 40, 43, 47, 52, 40, 47, 52, 40, 43, 47, 52, 47, 40, 47, 43, 40 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 120, 100, 110, 100, 120, 110, 100, 120, 100, 110, 100, 110, 120, 100, 110, 120 });

    // 25. 303 Tribute Gm - 303-style Gm
    addBassPattern ("303 Tribute Gm", 4,
        { 43, 46, 50, 55, 43, 50, 55, 43, 46, 50, 55, 50, 43, 50, 46, 43 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 },
        { 110, 90, 100, 90, 110, 100, 90, 110, 90, 100, 90, 100, 110, 90, 100, 110 });

    // --- Offbeat bass patterns ---

    // 26. Offbeat Hammer A - offbeat hits on A
    addBassPattern ("Offbeat Hammer A", 4,
        { 45, 45, 45, 45 },
        { 0.5, 1.5, 2.5, 3.5 },
        { 0.3, 0.3, 0.3, 0.3 },
        { 120, 120, 120, 120 });

    // 27. Offbeat Hammer C - offbeat C
    addBassPattern ("Offbeat Hammer C", 4,
        { 48, 48, 48, 48 },
        { 0.5, 1.5, 2.5, 3.5 },
        { 0.3, 0.3, 0.3, 0.3 },
        { 120, 110, 120, 110 });

    // 28. Swing Offbeat D - swung offbeat D
    addBassPattern ("Swing Offbeat D", 4,
        { 50, 50, 50, 50, 50, 50, 50, 50 },
        { 0.33, 0.67, 1.33, 1.67, 2.33, 2.67, 3.33, 3.67 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 },
        { 110, 90, 110, 90, 110, 90, 110, 90 });

    // 29. Syncopated Sub G - syncopated low G
    addBassPattern ("Syncopated Sub G", 4,
        { 43, 55, 43, 55, 43, 55, 43, 55 },
        { 0.0, 0.75, 1.0, 1.75, 2.0, 2.75, 3.0, 3.75 },
        { 0.5, 0.2, 0.5, 0.2, 0.5, 0.2, 0.5, 0.2 },
        { 120, 100, 120, 100, 120, 100, 120, 100 });

    // --- Driving 16th patterns ---

    // 30. 16th Machine Bass A - relentless 16ths on A
    addBassPattern ("16th Machine Bass A", 4,
        { 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 120, 80, 100, 80, 120, 80, 100, 80, 120, 80, 100, 80, 120, 80, 100, 80 });

    // 31. 16th Crusher C - 16th notes with accents on C
    addBassPattern ("16th Crusher C", 4,
        { 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 127, 70, 90, 70, 127, 70, 90, 70, 127, 70, 90, 70, 127, 70, 90, 70 });

    // 32. Turbo Bass Em - 16th with movement in Em
    addBassPattern ("Turbo Bass Em", 4,
        { 40, 40, 43, 40, 40, 40, 43, 40, 40, 40, 47, 40, 40, 40, 43, 40 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 120, 80, 110, 80, 120, 80, 110, 80, 120, 80, 120, 80, 120, 80, 110, 80 });

    // 33. Overdrive 16th Dm - heavy 16th Dm
    addBassPattern ("Overdrive 16th Dm", 4,
        { 50, 50, 53, 50, 50, 50, 53, 50, 50, 50, 57, 50, 50, 50, 53, 50 },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 127, 80, 110, 80, 127, 80, 110, 80, 127, 80, 120, 80, 127, 80, 110, 80 });

    // --- Additional bass patterns ---

    // 34. Freight Train Bass - driving A with 5th
    addBassPattern ("Freight Train Bass", 4,
        { 45, 52, 45, 52, 45, 52, 45, 52 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35 },
        { 120, 100, 120, 100, 120, 100, 120, 100 });

    // 35. Pile Driver Bass - aggressive C with hammering
    addBassPattern ("Pile Driver Bass", 4,
        { 48, 48, 48, 60, 48, 48, 48, 60 },
        { 0.0, 0.25, 0.5, 0.75, 2.0, 2.25, 2.5, 2.75 },
        { 0.2, 0.2, 0.2, 0.4, 0.2, 0.2, 0.2, 0.4 },
        { 127, 100, 110, 120, 127, 100, 110, 120 });

    // 36. Earthquake Sub - very low A with motion
    addBassPattern ("Earthquake Sub", 8,
        { 36, 38, 40, 38, 36, 38, 40, 38 },
        { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 },
        { 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8 },
        { 100, 95, 100, 95, 100, 95, 100, 95 });

    // 37. Rubber Band Bass - bouncy D bass
    addBassPattern ("Rubber Band Bass", 4,
        { 50, 57, 50, 57, 50, 57, 50, 57 },
        { 0.0, 0.25, 0.5, 0.75, 2.0, 2.25, 2.5, 2.75 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 },
        { 127, 90, 120, 90, 127, 90, 120, 90 });

    // 38. Flatline Bass - minimal single note E
    addBassPattern ("Flatline Bass", 4,
        { 40, 40, 40, 40, 40, 40, 40, 40 },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45 },
        { 100, 100, 100, 100, 100, 100, 100, 100 });

    // =========================================================================
    // KEYS / CHORD PATTERNS (38)
    // =========================================================================

    // --- Am chord progressions ---

    // Am = {57,60,64}  F = {53,57,60}  C = {48,52,55}  G = {55,59,62}

    // 1. Minor Stab Progression Am - Am F C G whole notes
    addChordPattern ("Minor Stab Progression Am", "Keys", 8,
        { {57,60,64}, {53,57,60}, {48,52,55}, {55,59,62} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 2. Dark Triad Am - Am F C half note stabs
    addChordPattern ("Dark Triad Am", "Keys", 4,
        { {57,60,64}, {53,57,60}, {48,52,55}, {55,59,62} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.8, 0.8, 0.8, 0.8 }, 110);

    // 3. Warehouse Chords Am - Am Em F C
    addChordPattern ("Warehouse Chords Am", "Keys", 8,
        { {57,60,64}, {52,55,59}, {53,57,60}, {48,52,55} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 4. Tunnel Vision Am - Am Dm Em Am (short stabs)
    addChordPattern ("Tunnel Vision Am", "Keys", 4,
        { {57,60,64}, {50,53,57}, {52,55,59}, {57,60,64} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 }, 120);

    // 5. Stab Machine Am - Am offbeat 8th stabs
    addChordPattern ("Stab Machine Am", "Keys", 4,
        { {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64},
          {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 }, 110);

    // --- Cm chord progressions ---

    // Cm = {60,63,67}  Eb = {63,67,70}  Bb = {58,62,65}  Gm = {55,58,62}

    // 6. Power Surge Chords Cm - Cm Eb Bb Gm
    addChordPattern ("Power Surge Chords Cm", "Keys", 8,
        { {60,63,67}, {63,67,70}, {58,62,65}, {55,58,62} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 7. Midnight Chords Cm - Cm Gm Ab Eb
    // Ab = {56,60,63}
    addChordPattern ("Midnight Chords Cm", "Keys", 8,
        { {60,63,67}, {55,58,62}, {56,60,63}, {63,67,70} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 8. Raving Stabs Cm - fast Cm stabs
    addChordPattern ("Raving Stabs Cm", "Keys", 4,
        { {60,63,67}, {60,63,67}, {60,63,67}, {60,63,67},
          {60,63,67}, {60,63,67}, {60,63,67}, {60,63,67} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 }, 120);

    // 9. Frozen Chord Cm - Cm with open voicing
    addChordPattern ("Frozen Chord Cm", "Keys", 8,
        { {48,63,67}, {51,67,70}, {46,62,65}, {43,58,62} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 90);

    // --- Dm chord progressions ---

    // Dm = {50,53,57}  Bb = {58,62,65}  F = {53,57,60}  C = {48,52,55}

    // 10. Dystopian Chords Dm - Dm Bb F C
    addChordPattern ("Dystopian Chords Dm", "Keys", 8,
        { {50,53,57}, {58,62,65}, {53,57,60}, {48,52,55} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 11. Industrial Keys Dm - Dm Am Bb Gm
    addChordPattern ("Industrial Keys Dm", "Keys", 8,
        { {50,53,57}, {57,60,64}, {58,62,65}, {55,58,62} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 12. Pounding Stabs Dm - Dm rhythmic stabs
    addChordPattern ("Pounding Stabs Dm", "Keys", 4,
        { {50,53,57}, {50,53,57}, {50,53,57}, {50,53,57},
          {50,53,57}, {50,53,57}, {50,53,57}, {50,53,57} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 }, 115);

    // --- Power chords ---

    // 13. Power Chord Crunch A - A5 power chord
    addChordPattern ("Power Chord Crunch A", "Keys", 4,
        { {57,64}, {57,64}, {57,64}, {57,64},
          {57,64}, {57,64}, {57,64}, {57,64} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 }, 120);

    // 14. Power Chord Crunch C - C5 power chord
    addChordPattern ("Power Chord Crunch C", "Keys", 4,
        { {48,55}, {48,55}, {48,55}, {48,55},
          {48,55}, {48,55}, {48,55}, {48,55} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3 }, 120);

    // 15. Power Chord Progression - A5 C5 D5 E5
    addChordPattern ("Power Chord Progression", "Keys", 8,
        { {57,64}, {48,55}, {50,57}, {52,59} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 110);

    // 16. Power Stab Sequence - quick power chord stabs
    addChordPattern ("Power Stab Sequence", "Keys", 4,
        { {57,64}, {48,55}, {50,57}, {52,59} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.3, 0.3, 0.3, 0.3 }, 127);

    // --- Suspended chords ---

    // 17. Suspended Tension Am - Asus2, Asus4 alternation
    // Asus2 = {57,59,64}  Asus4 = {57,62,64}
    addChordPattern ("Suspended Tension Am", "Keys", 8,
        { {57,59,64}, {57,62,64}, {57,59,64}, {57,62,64} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 100);

    // 18. Floating Sus Cm - Csus2, Csus4
    // Csus2 = {48,50,55}  Csus4 = {48,53,55}
    addChordPattern ("Floating Sus Cm", "Keys", 8,
        { {48,50,55}, {48,53,55}, {48,50,55}, {48,53,55} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 95);

    // 19. Unresolved Sus Dm - Dsus2, Dsus4
    // Dsus2 = {50,52,57}  Dsus4 = {50,55,57}
    addChordPattern ("Unresolved Sus Dm", "Keys", 8,
        { {50,52,57}, {50,55,57}, {50,52,57}, {50,55,57} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 95);

    // --- Diminished progressions ---

    // 20. Diminished Terror - Bdim, Ddim, Fdim
    // Bdim = {59,62,65}  Ddim = {50,53,56}  Fdim = {53,56,59}
    addChordPattern ("Diminished Terror", "Keys", 6,
        { {59,62,65}, {50,53,56}, {53,56,59} },
        { 0.0, 2.0, 4.0 },
        { 1.8, 1.8, 1.8 }, 110);

    // 21. Dim Descent - descending diminished chords
    // C#dim = {61,64,67}  Cdim = {60,63,66}  Bdim = {59,62,65}
    addChordPattern ("Dim Descent", "Keys", 6,
        { {61,64,67}, {60,63,66}, {59,62,65} },
        { 0.0, 2.0, 4.0 },
        { 1.8, 1.8, 1.8 }, 105);

    // 22. Anxiety Chords - dim with stab rhythm
    addChordPattern ("Anxiety Chords", "Keys", 4,
        { {59,62,65}, {59,62,65}, {50,53,56}, {50,53,56},
          {53,56,59}, {53,56,59}, {59,62,65}, {59,62,65} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 }, 110);

    // --- Open voicings ---

    // 23. Open Air Am - Am wide voicing
    addChordPattern ("Open Air Am", "Keys", 8,
        { {48,60,64,69}, {48,57,60,65}, {48,52,55,60}, {48,55,59,67} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 90);

    // 24. Cathedral Chords - open Cm voicing
    addChordPattern ("Cathedral Chords", "Keys", 8,
        { {48,60,63,72}, {51,63,67,72}, {53,58,65,70}, {48,55,62,67} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 85);

    // 25. Wide Horizon Dm - open Dm voicing
    addChordPattern ("Wide Horizon Dm", "Keys", 8,
        { {50,57,65,69}, {53,60,65,72}, {50,58,65,69}, {48,55,60,67} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 85);

    // --- Cluster chords ---

    // 26. Cluster Chaos Am - tight cluster voicings
    addChordPattern ("Cluster Chaos Am", "Keys", 4,
        { {60,61,64}, {60,61,64}, {59,60,64}, {59,60,64} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.5, 0.5, 0.5, 0.5 }, 110);

    // 27. Dissonant Cluster - chromatic cluster
    addChordPattern ("Dissonant Cluster", "Keys", 4,
        { {60,61,62,63}, {62,63,64,65}, {64,65,66,67}, {60,61,62,63} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.3, 0.3, 0.3, 0.3 }, 100);

    // 28. Noise Cluster - harsh cluster stabs
    addChordPattern ("Noise Cluster", "Keys", 4,
        { {60,61,63,66}, {60,61,63,66}, {62,63,65,68}, {62,63,65,68},
          {60,61,63,66}, {60,61,63,66}, {64,65,67,70}, {64,65,67,70} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 }, 120);

    // --- Tritone patterns ---

    // 29. Tritone Menace - tritone intervals
    addChordPattern ("Tritone Menace", "Keys", 4,
        { {60,66}, {62,68}, {64,70}, {60,66} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.8, 0.8, 0.8, 0.8 }, 110);

    // 30. Devil's Interval - tritone stabs
    addChordPattern ("Devil's Interval", "Keys", 4,
        { {60,66}, {60,66}, {62,68}, {62,68},
          {64,70}, {64,70}, {60,66}, {60,66} },
        { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5 },
        { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 }, 120);

    // 31. Tritone Crawl - slow tritone movement
    addChordPattern ("Tritone Crawl", "Keys", 8,
        { {48,54,60}, {50,56,62}, {52,58,64}, {48,54,60} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 95);

    // --- Additional chord patterns ---

    // 32. Hypnotic Minor Loop - Am Em repeating
    addChordPattern ("Hypnotic Minor Loop", "Keys", 4,
        { {57,60,64}, {52,55,59}, {57,60,64}, {52,55,59} },
        { 0.0, 1.0, 2.0, 3.0 },
        { 0.8, 0.8, 0.8, 0.8 }, 100);

    // 33. Underground Keys - Cm Fm progression
    // Fm = {53,56,60}
    addChordPattern ("Underground Keys", "Keys", 8,
        { {60,63,67}, {53,56,60}, {60,63,67}, {53,56,60} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 95);

    // 34. Rave Alarm Chords - syncopated Em stabs
    addChordPattern ("Rave Alarm Chords", "Keys", 4,
        { {52,55,59}, {52,55,59}, {52,55,59}, {52,55,59},
          {52,55,59}, {52,55,59}, {52,55,59}, {52,55,59} },
        { 0.25, 0.5, 1.25, 1.5, 2.25, 2.5, 3.25, 3.5 },
        { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 }, 120);

    // 35. Techno Noir - dark Am7 to Dm7
    // Am7 = {57,60,64,67}  Dm7 = {50,53,57,60}
    addChordPattern ("Techno Noir", "Keys", 8,
        { {57,60,64,67}, {50,53,57,60}, {57,60,64,67}, {50,53,57,60} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.8, 1.8, 1.8, 1.8 }, 90);

    // 36. Schranz Stab 01 - aggressive single chord stabs Am
    addChordPattern ("Schranz Stab 01", "Keys", 4,
        { {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64},
          {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64},
          {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64},
          {57,60,64}, {57,60,64}, {57,60,64}, {57,60,64} },
        { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
          2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75 },
        { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1,
          0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 }, 115);

    // 37. Parallel Minor Shift - Cm to C#m to Dm
    // C#m = {61,64,68}
    addChordPattern ("Parallel Minor Shift", "Keys", 6,
        { {60,63,67}, {61,64,68}, {50,53,57} },
        { 0.0, 2.0, 4.0 },
        { 1.8, 1.8, 1.8 }, 100);

    // 38. Afterhours Pads - soft Am progression for intros
    addChordPattern ("Afterhours Pads", "Keys", 8,
        { {57,60,64,69}, {53,57,60,65}, {48,52,55,60}, {52,55,59,64} },
        { 0.0, 2.0, 4.0, 6.0 },
        { 1.9, 1.9, 1.9, 1.9 }, 75);
}
