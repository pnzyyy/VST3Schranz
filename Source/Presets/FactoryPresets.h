#pragma once
#include <JuceHeader.h>
#include "PresetManager.h"

namespace FactoryPresets
{

// ---------------------------------------------------------------------------
// Deterministic PRNG for procedural preset generation
// ---------------------------------------------------------------------------
inline float pseudoRand(uint32_t& seed)
{
    seed = seed * 1664525u + 1013904223u;
    return static_cast<float>(seed & 0xFFFFu) / 65535.0f;
}

// Map 0-1 to an arbitrary range
inline float randRange(uint32_t& seed, float lo, float hi)
{
    return lo + pseudoRand(seed) * (hi - lo);
}

// Integer version inclusive
inline int randInt(uint32_t& seed, int lo, int hi)
{
    return lo + static_cast<int>(pseudoRand(seed) * (hi - lo + 0.999f));
}

// Clamp helper
inline float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// ---------------------------------------------------------------------------
// make() — creates a PresetData with ALL parameters including new effects
// ---------------------------------------------------------------------------
inline PresetData make(const juce::String& name,
                       // Oscillators
                       int osc1Type, float osc1Gain, float osc1Detune,
                       int osc2Type, float osc2Gain, float osc2Detune,
                       // Amp envelope
                       float ampAttack, float ampDecay, float ampSustain, float ampRelease,
                       // Distortion
                       int distType, float distDrive, float distMix,
                       // Bitcrusher
                       float crushBits, float crushRate,
                       // Filter
                       int filterType, float filterCutoff, float filterResonance, float filterEnvAmount,
                       // Delay
                       float delayTime, float delayFeedback, float delayMix, int delayPingPong,
                       // Reverb
                       float reverbSize, float reverbDamping, float reverbMix, float reverbWidth,
                       // Compressor
                       float compThreshold, float compRatio, float compAttack, float compRelease, float compMakeup,
                       // Chorus
                       float chorusRate, float chorusDepth, float chorusMix,
                       // Master
                       float masterGain,
                       // Phaser (new)
                       float phaserRate = 0.5f, float phaserDepth = 0.0f,
                       float phaserMix = 0.0f, float phaserFeedback = 0.3f,
                       // EQ (new)
                       float eqLowGain = 0.0f, float eqMidGain = 0.0f,
                       float eqHighGain = 0.0f, float eqMidFreq = 1000.0f,
                       // Ring mod (new)
                       float ringModFreq = 440.0f, float ringModMix = 0.0f,
                       // Waveshaper (new)
                       float wsAmount = 0.0f, int wsType = 0,
                       // Unison (new)
                       int unisonVoices = 1, float unisonDetune = 15.0f)
{
    PresetData p;
    p.name = name;
    p.state = juce::ValueTree("Preset");
    auto& s = p.state;

    s.setProperty("unisonVoices",    unisonVoices, nullptr);
    s.setProperty("unisonDetune",    unisonDetune, nullptr);
    s.setProperty("osc1Type",        osc1Type, nullptr);
    s.setProperty("osc1Gain",        osc1Gain, nullptr);
    s.setProperty("osc1Detune",      osc1Detune, nullptr);
    s.setProperty("osc2Type",        osc2Type, nullptr);
    s.setProperty("osc2Gain",        osc2Gain, nullptr);
    s.setProperty("osc2Detune",      osc2Detune, nullptr);

    s.setProperty("ampAttack",       ampAttack, nullptr);
    s.setProperty("ampDecay",        ampDecay, nullptr);
    s.setProperty("ampSustain",      ampSustain, nullptr);
    s.setProperty("ampRelease",      ampRelease, nullptr);

    s.setProperty("distType",        distType, nullptr);
    s.setProperty("distDrive",       distDrive, nullptr);
    s.setProperty("distMix",         distMix, nullptr);

    s.setProperty("crushBits",       crushBits, nullptr);
    s.setProperty("crushRate",       crushRate, nullptr);

    s.setProperty("filterType",      filterType, nullptr);
    s.setProperty("filterCutoff",    filterCutoff, nullptr);
    s.setProperty("filterResonance", filterResonance, nullptr);
    s.setProperty("filterEnvAmount", filterEnvAmount, nullptr);

    s.setProperty("delayTime",       delayTime, nullptr);
    s.setProperty("delayFeedback",   delayFeedback, nullptr);
    s.setProperty("delayMix",        delayMix, nullptr);
    s.setProperty("delayPingPong",   delayPingPong, nullptr);

    s.setProperty("reverbSize",      reverbSize, nullptr);
    s.setProperty("reverbDamping",   reverbDamping, nullptr);
    s.setProperty("reverbMix",       reverbMix, nullptr);
    s.setProperty("reverbWidth",     reverbWidth, nullptr);

    s.setProperty("compThreshold",   compThreshold, nullptr);
    s.setProperty("compRatio",       compRatio, nullptr);
    s.setProperty("compAttack",      compAttack, nullptr);
    s.setProperty("compRelease",     compRelease, nullptr);
    s.setProperty("compMakeup",      compMakeup, nullptr);

    s.setProperty("chorusRate",      chorusRate, nullptr);
    s.setProperty("chorusDepth",     chorusDepth, nullptr);
    s.setProperty("chorusMix",       chorusMix, nullptr);

    s.setProperty("masterGain",      masterGain, nullptr);

    s.setProperty("phaserRate",      phaserRate, nullptr);
    s.setProperty("phaserDepth",     phaserDepth, nullptr);
    s.setProperty("phaserMix",       phaserMix, nullptr);
    s.setProperty("phaserFeedback",  phaserFeedback, nullptr);

    s.setProperty("eqLowGain",      eqLowGain, nullptr);
    s.setProperty("eqMidGain",      eqMidGain, nullptr);
    s.setProperty("eqHighGain",     eqHighGain, nullptr);
    s.setProperty("eqMidFreq",      eqMidFreq, nullptr);

    s.setProperty("ringModFreq",    ringModFreq, nullptr);
    s.setProperty("ringModMix",     ringModMix, nullptr);

    s.setProperty("wsAmount",       wsAmount, nullptr);
    s.setProperty("wsType",         wsType, nullptr);

    return p;
}

// ---------------------------------------------------------------------------
// Name generation helpers
// ---------------------------------------------------------------------------
inline juce::String kickName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Demolition", "Concrete", "Bunker", "Piledriver", "Jackhammer",
        "Thunder", "Sledgehammer", "Stomp", "Impact", "Wrecker",
        "Bomb", "Seismic", "Quake", "Crusher", "Tank",
        "Anvil", "Pummel", "Smash", "Blitz", "Blast"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Kick " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String bassName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Punishment", "Rumble", "Subterranean", "Dark Matter", "Depths",
        "Abyss", "Tectonic", "Grinding", "Filthy", "Massive",
        "Low Blow", "Foundation", "Undertow", "Pressure", "Subsonic",
        "Iron", "Vault", "Cellar", "Gut Punch", "Tremor"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Bass " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String leadName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Razor", "Searing", "Lethal", "Voltage", "Neon",
        "Screech", "Laser", "Plasma", "Shred", "Fury",
        "Blade", "Assault", "Venom", "Chrome", "Piercing",
        "Wired", "Scorched", "Barbed", "Shriek", "Flash"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Lead " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String stabName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Warehouse", "Rave", "Strobe", "Punchy", "Blunt",
        "Spike", "Jab", "Trigger", "Snare", "Snap",
        "Clip", "Burst", "Pulse", "Crack", "Strike",
        "Shock", "Salvo", "Hit", "Clang", "Chop"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Stab " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String padName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Frozen", "Void", "Drift", "Endless", "Spectral",
        "Fog", "Dissolve", "Haze", "Etheric", "Hollow",
        "Glacier", "Eclipse", "Phantom", "Murky", "Dusk",
        "Twilight", "Shadow", "Nebula", "Silence", "Distant"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Pad " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String acidName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Acid", "Squelch", "Resonant", "303", "Corrode",
        "Dissolve", "Toxic", "Caustic", "Erode", "Melt",
        "Chemical", "Burn", "Boil", "Drip", "Flux",
        "Liquid", "Etch", "Bubble", "React", "Fume"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Acid " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String industrialName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Factory", "Grinder", "Rust", "Furnace", "Piston",
        "Turbine", "Wreckage", "Scrap", "Slag", "Iron",
        "Welded", "Corroded", "Machine", "Rivet", "Forge",
        "Molten", "Smelter", "Gearbox", "Boiler", "Oxide"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Industrial " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String fxName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Wasteland", "Corridor", "Tunnel", "Static", "Feedback",
        "Glitch", "Signal", "Artifact", "Interference", "Remnant",
        "Fragment", "Debris", "Residue", "Distortion", "Anomaly",
        "Warp", "Rift", "Fracture", "Void", "Ghost"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " FX " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String percName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Metal", "Click", "Clap", "Rim", "Tom",
        "Shaker", "Zap", "Tap", "Knock", "Ping",
        "Thud", "Pop", "Tick", "Clink", "Rattle",
        "Crash", "Bell", "Clave", "Bongo", "Cowbell"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Perc " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String screamName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Rave", "Alarm", "Siren", "Mayhem", "Fury",
        "Havoc", "Rampage", "Frenzy", "Riot", "Carnage",
        "Overdrive", "Meltdown", "Eruption", "Inferno", "Chaos",
        "Madness", "Berserk", "Assault", "Rage", "Storm"
    };
    int pi = randInt(seed, 0, 19);
    return juce::String(prefixes[pi]) + " Scream " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String darkAmbientName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Abyssal", "Crypt", "Catacombs", "Underworld", "Obsidian",
        "Monolith", "Void", "Stygian", "Desolate", "Forgotten",
        "Ruined", "Submerged", "Buried", "Eternal", "Forsaken"
    };
    int pi = randInt(seed, 0, 14);
    return juce::String(prefixes[pi]) + " Dark " + juce::String(idx + 1).paddedLeft('0', 2);
}

inline juce::String experimentalName(int idx, uint32_t& seed)
{
    const char* prefixes[] = {
        "Mutation", "Splice", "Paradox", "Entropy", "Quantum",
        "Fractal", "Glitch", "Corrupt", "Unstable", "Anomaly",
        "Diverge", "Morph", "Warp", "Twist", "Flux"
    };
    int pi = randInt(seed, 0, 14);
    return juce::String(prefixes[pi]) + " Exp " + juce::String(idx + 1).paddedLeft('0', 2);
}

// ---------------------------------------------------------------------------
// Category generator functions
// ---------------------------------------------------------------------------

inline void generateKicks(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 100001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = kickName(i, seed);

        int o1t   = randInt(seed, 0, 1);             // sine or saw
        float o1g = randRange(seed, 0.85f, 1.0f);
        float o1d = 0.0f;
        int o2t   = randInt(seed, 0, 1);             // sine/saw sub layer
        float o2g = randRange(seed, 0.0f, 0.25f);
        float o2d = 0.0f;

        float att  = randRange(seed, 0.001f, 0.004f);
        float dec  = randRange(seed, 0.08f, 0.3f);
        float sus  = 0.0f;
        float rel  = randRange(seed, 0.02f, 0.1f);

        int dt     = randInt(seed, 0, 1);            // tanh / soft clip punch
        float ddr  = randRange(seed, 4.0f, 12.0f);
        float dmx  = clampf(randRange(seed, 0.5f, 0.7f) - (ddr - 4.0f) * 0.012f, 0.4f, 0.7f);

        float cb   = 16.0f;                          // bitcrush off for clean low end
        float cr   = 1.0f;

        int ft     = 0; // LP always for kicks
        float fc   = randRange(seed, 80.0f, 300.0f);
        float fr   = randRange(seed, 0.5f, 2.0f);    // low res to avoid whine
        float fe   = randRange(seed, 0.3f, 0.7f);    // pitch-envelope feel

        float dlt  = randRange(seed, 10.0f, 50.0f);
        float dlf  = 0.0f;
        float dlm  = 0.0f;
        int dlp    = 0;

        float rvs  = randRange(seed, 0.0f, 0.1f);
        float rvd  = randRange(seed, 0.3f, 0.6f);
        float rvm  = randRange(seed, 0.0f, 0.05f);
        float rvw  = randRange(seed, 0.0f, 0.2f);

        float ct   = randRange(seed, -18.0f, -6.0f);
        float cra  = randRange(seed, 3.0f, 8.0f);
        float ca   = randRange(seed, 0.1f, 4.0f);
        float crl  = randRange(seed, 20.0f, 80.0f);
        float cm   = randRange(seed, 1.0f, 5.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.75f, 0.85f);

        // EQ: boost low, gently cut mid/high
        float eqL  = randRange(seed, 2.0f, 6.0f);
        float eqM  = randRange(seed, -3.0f, 0.0f);
        float eqH  = randRange(seed, -5.0f, -1.0f);
        float eqMF = randRange(seed, 400.0f, 1000.0f);

        // waveshaper occasionally, subtle
        float wsa  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.15f, 0.35f) : 0.0f;
        int wst    = 0; // tanh warmth

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         // new params at defaults except EQ and ws
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         wsa, wst));
    }
}

inline void generateBass(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 200001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = bassName(i, seed);

        int o1t   = randInt(seed, 0, 2);              // sine/saw/square
        float o1g = randRange(seed, 0.8f, 1.0f);
        float o1d = randRange(seed, -8.0f, 8.0f);
        int o2t   = randInt(seed, 0, 1);              // sine/saw sub
        float o2g = randRange(seed, 0.2f, 0.5f);
        float o2d = randRange(seed, -6.0f, 6.0f);

        float att  = randRange(seed, 0.001f, 0.015f);
        float dec  = randRange(seed, 0.1f, 0.4f);
        float sus  = randRange(seed, 0.4f, 0.8f);
        float rel  = randRange(seed, 0.05f, 0.25f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 3.0f, 10.0f);
        float dmx  = clampf(randRange(seed, 0.35f, 0.6f) - (ddr - 3.0f) * 0.01f, 0.3f, 0.6f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = 0;  // LP for sub-focused bass
        float fc   = randRange(seed, 150.0f, 900.0f);
        float fr   = randRange(seed, 0.5f, 3.0f);
        float fe   = randRange(seed, 0.0f, 0.4f);

        float dlt  = randRange(seed, 10.0f, 100.0f);
        float dlf  = 0.0f;
        float dlm  = 0.0f;
        int dlp    = 0;

        float rvs  = randRange(seed, 0.0f, 0.15f);
        float rvd  = randRange(seed, 0.3f, 0.6f);
        float rvm  = randRange(seed, 0.0f, 0.05f);
        float rvw  = randRange(seed, 0.0f, 0.2f);

        float ct   = randRange(seed, -28.0f, -8.0f);
        float cra  = randRange(seed, 2.0f, 6.0f);
        float ca   = randRange(seed, 0.5f, 10.0f);
        float crl  = randRange(seed, 30.0f, 120.0f);
        float cm   = randRange(seed, 1.0f, 5.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.72f, 0.85f);

        float eqL  = randRange(seed, 1.0f, 5.0f);
        float eqM  = randRange(seed, -2.0f, 2.0f);
        float eqH  = randRange(seed, -5.0f, -2.0f);
        float eqMF = randRange(seed, 300.0f, 800.0f);

        float wsa  = pseudoRand(seed) > 0.6f ? randRange(seed, 0.1f, 0.3f) : 0.0f;
        int wst    = 0;

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         wsa, wst));
    }
}

inline void generateLeads(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 300001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = leadName(i, seed);

        int o1t   = randInt(seed, 1, 2);              // saw or square
        float o1g = randRange(seed, 0.6f, 0.85f);
        float o1d = randRange(seed, -3.0f, 3.0f);
        int o2t   = randInt(seed, 1, 2);
        float o2g = randRange(seed, 0.3f, 0.6f);
        float o2d = o1d + randRange(seed, -15.0f, 15.0f);  // subtle detune for richness
        o2d = clampf(o2d, -18.0f, 18.0f);

        float att  = randRange(seed, 0.002f, 0.02f);
        float dec  = randRange(seed, 0.05f, 0.3f);
        float sus  = randRange(seed, 0.5f, 0.85f);
        float rel  = randRange(seed, 0.05f, 0.25f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 3.0f, 10.0f);
        float dmx  = clampf(randRange(seed, 0.35f, 0.6f) - (ddr - 3.0f) * 0.01f, 0.3f, 0.6f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = randInt(seed, 0, 2);  // LP/HP/BP
        float fc   = randRange(seed, 1500.0f, 7000.0f);
        float fr   = randRange(seed, 1.0f, 3.0f);
        float fe   = randRange(seed, 0.1f, 0.5f);

        float dlt  = randRange(seed, 80.0f, 300.0f);
        float dlf  = randRange(seed, 0.15f, 0.4f);
        float dlm  = randRange(seed, 0.1f, 0.25f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.2f, 0.5f);
        float rvd  = randRange(seed, 0.2f, 0.5f);
        float rvm  = randRange(seed, 0.1f, 0.25f);
        float rvw  = randRange(seed, 0.3f, 0.7f);

        float ct   = randRange(seed, -18.0f, -5.0f);
        float cra  = randRange(seed, 2.0f, 6.0f);
        float ca   = randRange(seed, 1.0f, 15.0f);
        float crl  = randRange(seed, 30.0f, 100.0f);
        float cm   = randRange(seed, 0.0f, 5.0f);

        float chr  = randRange(seed, 0.2f, 1.5f);
        float chd  = randRange(seed, 0.1f, 0.3f);
        float chm  = randRange(seed, 0.1f, 0.25f);

        float mg   = randRange(seed, 0.65f, 0.82f);

        // phaser on some leads, subtle
        float phR  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.3f, 1.5f) : 0.5f;
        float phD  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.2f, 0.5f) : 0.0f;
        float phM  = phD > 0.0f ? randRange(seed, 0.1f, 0.3f) : 0.0f;
        float phF  = randRange(seed, 0.1f, 0.5f);

        float eqL  = randRange(seed, -3.0f, 1.0f);
        float eqM  = randRange(seed, -1.0f, 3.0f);
        float eqH  = randRange(seed, 0.0f, 5.0f);
        float eqMF = randRange(seed, 1000.0f, 5000.0f);

        float wsa  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.1f, 0.3f) : 0.0f;
        int wst    = 0;

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         wsa, wst));
    }
}

inline void generateStabs(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 400001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = stabName(i, seed);

        int o1t   = randInt(seed, 1, 2);
        float o1g = randRange(seed, 0.7f, 0.95f);
        float o1d = randRange(seed, -5.0f, 5.0f);
        int o2t   = randInt(seed, 1, 2);
        float o2g = randRange(seed, 0.3f, 0.55f);
        float o2d = randRange(seed, -10.0f, 10.0f);

        float att  = randRange(seed, 0.001f, 0.005f);
        float dec  = randRange(seed, 0.1f, 0.25f);
        float sus  = randRange(seed, 0.0f, 0.3f);
        float rel  = randRange(seed, 0.03f, 0.12f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 3.0f, 8.0f);
        float dmx  = clampf(randRange(seed, 0.35f, 0.6f) - (ddr - 3.0f) * 0.01f, 0.3f, 0.6f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = randInt(seed, 0, 2);
        float fc   = randRange(seed, 800.0f, 4000.0f);
        float fr   = randRange(seed, 0.5f, 3.0f);
        float fe   = randRange(seed, 0.2f, 0.6f);

        float dlt  = randRange(seed, 80.0f, 250.0f);
        float dlf  = randRange(seed, 0.1f, 0.35f);
        float dlm  = randRange(seed, 0.0f, 0.2f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.1f, 0.3f);
        float rvd  = randRange(seed, 0.3f, 0.6f);
        float rvm  = randRange(seed, 0.05f, 0.2f);
        float rvw  = randRange(seed, 0.2f, 0.6f);

        float ct   = randRange(seed, -15.0f, -3.0f);
        float cra  = randRange(seed, 3.0f, 8.0f);
        float ca   = randRange(seed, 0.1f, 5.0f);
        float crl  = randRange(seed, 15.0f, 60.0f);
        float cm   = randRange(seed, 1.0f, 5.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.65f, 0.82f);

        float eqL  = randRange(seed, -2.0f, 2.0f);
        float eqM  = randRange(seed, 0.0f, 3.0f);
        float eqH  = randRange(seed, -2.0f, 3.0f);
        float eqMF = randRange(seed, 800.0f, 3000.0f);

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         0.0f, 0));
    }
}

inline void generatePads(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 500001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = padName(i, seed);

        int o1t   = randInt(seed, 0, 2);
        float o1g = randRange(seed, 0.4f, 0.65f);
        float o1d = randRange(seed, -8.0f, 8.0f);
        int o2t   = randInt(seed, 0, 2);
        float o2g = randRange(seed, 0.3f, 0.55f);
        float o2d = o1d + randRange(seed, -14.0f, 14.0f);  // gentle detune for width
        o2d = clampf(o2d, -18.0f, 18.0f);

        float att  = randRange(seed, 0.1f, 0.6f);
        float dec  = randRange(seed, 0.3f, 1.0f);
        float sus  = randRange(seed, 0.6f, 0.9f);
        float rel  = randRange(seed, 0.8f, 3.0f);

        int dt     = 0;  // minimal distortion
        float ddr  = randRange(seed, 1.0f, 4.0f);
        float dmx  = randRange(seed, 0.1f, 0.3f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = 0;  // warm LP
        float fc   = randRange(seed, 1000.0f, 5000.0f);
        float fr   = randRange(seed, 0.3f, 1.5f);
        float fe   = randRange(seed, -0.2f, 0.3f);

        float dlt  = randRange(seed, 150.0f, 500.0f);
        float dlf  = randRange(seed, 0.2f, 0.45f);
        float dlm  = randRange(seed, 0.1f, 0.3f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.4f, 0.8f);
        float rvd  = randRange(seed, 0.2f, 0.5f);
        float rvm  = randRange(seed, 0.2f, 0.4f);
        float rvw  = randRange(seed, 0.6f, 1.0f);

        float ct   = randRange(seed, -25.0f, -10.0f);
        float cra  = randRange(seed, 1.5f, 4.0f);
        float ca   = randRange(seed, 5.0f, 50.0f);
        float crl  = randRange(seed, 50.0f, 200.0f);
        float cm   = randRange(seed, 0.0f, 3.0f);

        float chr  = randRange(seed, 0.3f, 2.0f);
        float chd  = randRange(seed, 0.2f, 0.5f);
        float chm  = randRange(seed, 0.2f, 0.4f);

        float mg   = randRange(seed, 0.55f, 0.78f);

        // phaser for pads, subtle
        float phR  = randRange(seed, 0.1f, 1.0f);
        float phD  = randRange(seed, 0.2f, 0.5f);
        float phM  = randRange(seed, 0.1f, 0.3f);
        float phF  = randRange(seed, 0.2f, 0.5f);

        float eqL  = randRange(seed, -2.0f, 2.0f);
        float eqM  = randRange(seed, -2.0f, 2.0f);
        float eqH  = randRange(seed, -4.0f, 2.0f);
        float eqMF = randRange(seed, 500.0f, 3000.0f);

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         0.0f, 0));
    }
}

inline void generateAcid(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 600001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = acidName(i, seed);

        int o1t   = 1;  // saw always for acid
        float o1g = randRange(seed, 0.75f, 1.0f);
        float o1d = randRange(seed, -4.0f, 4.0f);
        int o2t   = randInt(seed, 1, 2);  // saw or square
        float o2g = randRange(seed, 0.0f, 0.3f);
        float o2d = randRange(seed, -10.0f, 10.0f);

        float att  = randRange(seed, 0.001f, 0.01f);
        float dec  = randRange(seed, 0.1f, 0.35f);
        float sus  = randRange(seed, 0.2f, 0.5f);
        float rel  = randRange(seed, 0.05f, 0.18f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 5.0f, 12.0f);
        float dmx  = clampf(randRange(seed, 0.45f, 0.6f) - (ddr - 5.0f) * 0.008f, 0.4f, 0.6f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = 0;  // LP always for acid
        float fc   = randRange(seed, 400.0f, 2000.0f);
        float fr   = randRange(seed, 4.0f, 8.0f);       // resonant 303 character
        float fe   = randRange(seed, 0.4f, 0.8f);        // strong env sweep

        float dlt  = randRange(seed, 90.0f, 200.0f);
        float dlf  = randRange(seed, 0.2f, 0.4f);
        float dlm  = randRange(seed, 0.1f, 0.25f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.05f, 0.2f);
        float rvd  = randRange(seed, 0.3f, 0.6f);
        float rvm  = randRange(seed, 0.0f, 0.12f);
        float rvw  = randRange(seed, 0.1f, 0.4f);

        float ct   = randRange(seed, -15.0f, -5.0f);
        float cra  = randRange(seed, 2.0f, 6.0f);
        float ca   = randRange(seed, 1.0f, 10.0f);
        float crl  = randRange(seed, 30.0f, 100.0f);
        float cm   = randRange(seed, 0.0f, 4.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.68f, 0.82f);

        // subtle phaser for acid movement
        float phR  = pseudoRand(seed) > 0.6f ? randRange(seed, 0.3f, 1.5f) : 0.5f;
        float phD  = pseudoRand(seed) > 0.6f ? randRange(seed, 0.2f, 0.4f) : 0.0f;
        float phM  = phD > 0.0f ? randRange(seed, 0.1f, 0.3f) : 0.0f;
        float phF  = randRange(seed, 0.2f, 0.5f);

        float eqL  = randRange(seed, -1.0f, 2.0f);
        float eqM  = randRange(seed, 1.0f, 4.0f);
        float eqH  = randRange(seed, -2.0f, 3.0f);
        float eqMF = randRange(seed, 800.0f, 4000.0f);

        float wsa  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.1f, 0.3f) : 0.0f;
        int wst    = 0; // tanh for acid warmth

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         440.0f, 0.0f,
                         wsa, wst));
    }
}

inline void generateIndustrial(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 700001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = industrialName(i, seed);

        int o1t   = randInt(seed, 1, 3);              // saw/square/noise
        float o1g = randRange(seed, 0.6f, 0.95f);
        float o1d = randRange(seed, -12.0f, 12.0f);
        int o2t   = randInt(seed, 2, 3);              // square or noise
        float o2g = randRange(seed, 0.25f, 0.55f);
        float o2d = randRange(seed, -15.0f, 15.0f);

        float att  = randRange(seed, 0.001f, 0.03f);
        float dec  = randRange(seed, 0.05f, 0.35f);
        float sus  = randRange(seed, 0.2f, 0.6f);
        float rel  = randRange(seed, 0.05f, 0.3f);

        int dt     = randInt(seed, 0, 3);  // tanh/softclip/foldback/bitcrush
        float ddr  = randRange(seed, 8.0f, 18.0f);
        float dmx  = clampf(randRange(seed, 0.6f, 0.7f) - (ddr - 8.0f) * 0.012f, 0.5f, 0.7f);

        float cb   = randRange(seed, 5.0f, 10.0f);   // controlled bitcrush
        float cr   = randRange(seed, 2.0f, 4.0f);

        int ft     = randInt(seed, 0, 2);
        float fc   = randRange(seed, 600.0f, 6000.0f);
        float fr   = randRange(seed, 1.0f, 4.0f);
        float fe   = randRange(seed, -0.4f, 0.4f);

        float dlt  = randRange(seed, 30.0f, 200.0f);
        float dlf  = randRange(seed, 0.1f, 0.4f);
        float dlm  = randRange(seed, 0.05f, 0.25f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.15f, 0.5f);
        float rvd  = randRange(seed, 0.2f, 0.5f);
        float rvm  = randRange(seed, 0.1f, 0.25f);
        float rvw  = randRange(seed, 0.2f, 0.6f);

        float ct   = randRange(seed, -20.0f, -5.0f);
        float cra  = randRange(seed, 4.0f, 10.0f);
        float ca   = randRange(seed, 0.1f, 5.0f);
        float crl  = randRange(seed, 15.0f, 60.0f);
        float cm   = randRange(seed, 1.0f, 6.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.6f, 0.8f);

        // ring mod for metallic but tuned character
        float rmF  = randRange(seed, 50.0f, 400.0f);
        float rmM  = randRange(seed, 0.1f, 0.4f);

        // waveshaper for grit, moderate
        float wsa  = randRange(seed, 0.2f, 0.5f);
        int wst    = randInt(seed, 0, 1);

        float eqL  = randRange(seed, -2.0f, 4.0f);
        float eqM  = randRange(seed, -3.0f, 5.0f);
        float eqH  = randRange(seed, -4.0f, 3.0f);
        float eqMF = randRange(seed, 500.0f, 4000.0f);

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         wsa, wst));
    }
}

inline void generateFX(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 800001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = fxName(i, seed);

        int o1t   = randInt(seed, 0, 3);
        float o1g = randRange(seed, 0.3f, 0.65f);
        float o1d = randRange(seed, -24.0f, 24.0f);
        int o2t   = randInt(seed, 0, 3);
        float o2g = randRange(seed, 0.2f, 0.5f);
        float o2d = randRange(seed, -30.0f, 30.0f);

        float att  = randRange(seed, 0.05f, 1.0f);
        float dec  = randRange(seed, 0.3f, 1.5f);
        float sus  = randRange(seed, 0.3f, 0.7f);
        float rel  = randRange(seed, 0.5f, 4.0f);

        int dt     = randInt(seed, 0, 2);
        float ddr  = randRange(seed, 2.0f, 12.0f);
        float dmx  = clampf(randRange(seed, 0.3f, 0.5f) - (ddr - 2.0f) * 0.008f, 0.2f, 0.5f);

        float cb   = randRange(seed, 8.0f, 16.0f);
        float cr   = randRange(seed, 1.0f, 3.0f);

        int ft     = randInt(seed, 0, 2);
        float fc   = randRange(seed, 400.0f, 8000.0f);
        float fr   = randRange(seed, 0.5f, 4.0f);
        float fe   = randRange(seed, -0.6f, 0.6f);   // sweeps

        float dlt  = randRange(seed, 100.0f, 600.0f);
        float dlf  = randRange(seed, 0.4f, 0.7f);
        float dlm  = randRange(seed, 0.3f, 0.5f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.4f, 0.9f);
        float rvd  = randRange(seed, 0.1f, 0.5f);
        float rvm  = randRange(seed, 0.3f, 0.5f);
        float rvw  = randRange(seed, 0.5f, 1.0f);

        float ct   = randRange(seed, -28.0f, -10.0f);
        float cra  = randRange(seed, 1.5f, 4.0f);
        float ca   = randRange(seed, 5.0f, 80.0f);
        float crl  = randRange(seed, 50.0f, 300.0f);
        float cm   = randRange(seed, 0.0f, 3.0f);

        float chr  = randRange(seed, 0.2f, 3.0f);
        float chd  = randRange(seed, 0.1f, 0.6f);
        float chm  = randRange(seed, 0.1f, 0.4f);

        float mg   = randRange(seed, 0.5f, 0.72f);

        float phR  = randRange(seed, 0.1f, 2.0f);
        float phD  = randRange(seed, 0.1f, 0.6f);
        float phM  = randRange(seed, 0.1f, 0.4f);
        float phF  = randRange(seed, 0.1f, 0.6f);

        float eqL  = randRange(seed, -4.0f, 4.0f);
        float eqM  = randRange(seed, -4.0f, 4.0f);
        float eqH  = randRange(seed, -4.0f, 4.0f);
        float eqMF = randRange(seed, 300.0f, 6000.0f);

        float rmF  = pseudoRand(seed) > 0.5f ? randRange(seed, 50.0f, 500.0f) : 440.0f;
        float rmM  = pseudoRand(seed) > 0.5f ? randRange(seed, 0.1f, 0.4f) : 0.0f;

        float wsa  = pseudoRand(seed) > 0.5f ? randRange(seed, 0.1f, 0.4f) : 0.0f;
        int wst    = randInt(seed, 0, 1);

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         wsa, wst));
    }
}

inline void generatePercussion(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 900001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = percName(i, seed);

        int o1t   = randInt(seed, 0, 3);
        float o1g = randRange(seed, 0.7f, 1.0f);
        float o1d = randRange(seed, -8.0f, 8.0f);
        int o2t   = randInt(seed, 0, 3);
        float o2g = randRange(seed, 0.0f, 0.4f);
        float o2d = randRange(seed, -15.0f, 15.0f);

        float att  = randRange(seed, 0.001f, 0.004f);
        float dec  = randRange(seed, 0.05f, 0.2f);
        float sus  = 0.0f;
        float rel  = randRange(seed, 0.01f, 0.08f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 2.0f, 9.0f);
        float dmx  = clampf(randRange(seed, 0.3f, 0.5f) - (ddr - 2.0f) * 0.008f, 0.2f, 0.5f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = randInt(seed, 1, 2);  // HP or BP for tone
        float fc   = randRange(seed, 800.0f, 6000.0f);
        float fr   = randRange(seed, 0.5f, 3.0f);
        float fe   = randRange(seed, 0.1f, 0.5f);

        float dlt  = randRange(seed, 10.0f, 100.0f);
        float dlf  = randRange(seed, 0.0f, 0.25f);
        float dlm  = randRange(seed, 0.0f, 0.12f);
        int dlp    = 0;

        float rvs  = randRange(seed, 0.05f, 0.3f);
        float rvd  = randRange(seed, 0.3f, 0.6f);
        float rvm  = randRange(seed, 0.0f, 0.12f);
        float rvw  = randRange(seed, 0.1f, 0.5f);

        float ct   = randRange(seed, -15.0f, -3.0f);
        float cra  = randRange(seed, 3.0f, 8.0f);
        float ca   = randRange(seed, 0.1f, 3.0f);
        float crl  = randRange(seed, 10.0f, 50.0f);
        float cm   = randRange(seed, 1.0f, 5.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.65f, 0.85f);

        float eqL  = randRange(seed, -3.0f, 2.0f);
        float eqM  = randRange(seed, -2.0f, 3.0f);
        float eqH  = randRange(seed, -1.0f, 4.0f);
        float eqMF = randRange(seed, 500.0f, 5000.0f);

        // ring mod occasionally for metallic perc, tuned
        float rmF  = pseudoRand(seed) > 0.6f ? randRange(seed, 200.0f, 800.0f) : 440.0f;
        float rmM  = pseudoRand(seed) > 0.6f ? randRange(seed, 0.1f, 0.3f) : 0.0f;

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         0.0f, 0));
    }
}

inline void generateScream(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 1000001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = screamName(i, seed);

        int o1t   = 1;                                // saw for hoover
        float o1g = randRange(seed, 0.8f, 1.0f);
        float o1d = randRange(seed, -7.0f, 7.0f);
        int o2t   = randInt(seed, 1, 2);
        float o2g = randRange(seed, 0.5f, 0.8f);
        float o2d = o1d + randRange(seed, -16.0f, 16.0f);  // detuned second saw
        o2d = clampf(o2d, -22.0f, 22.0f);

        float att  = randRange(seed, 0.001f, 0.01f);
        float dec  = randRange(seed, 0.05f, 0.2f);
        float sus  = randRange(seed, 0.6f, 0.9f);
        float rel  = randRange(seed, 0.05f, 0.2f);

        int dt     = randInt(seed, 0, 1);
        float ddr  = randRange(seed, 10.0f, 18.0f);   // aggressive but controlled
        float dmx  = clampf(randRange(seed, 0.5f, 0.6f) - (ddr - 10.0f) * 0.012f, 0.4f, 0.6f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = randInt(seed, 0, 1);
        float fc   = randRange(seed, 1500.0f, 6000.0f);
        float fr   = randRange(seed, 2.0f, 4.0f);
        float fe   = randRange(seed, 0.2f, 0.6f);

        float dlt  = randRange(seed, 60.0f, 150.0f);
        float dlf  = randRange(seed, 0.1f, 0.35f);
        float dlm  = randRange(seed, 0.0f, 0.2f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.1f, 0.3f);
        float rvd  = randRange(seed, 0.2f, 0.5f);
        float rvm  = randRange(seed, 0.05f, 0.2f);
        float rvw  = randRange(seed, 0.2f, 0.6f);

        float ct   = randRange(seed, -12.0f, -3.0f);
        float cra  = randRange(seed, 4.0f, 8.0f);
        float ca   = randRange(seed, 0.1f, 3.0f);
        float crl  = randRange(seed, 10.0f, 40.0f);
        float cm   = randRange(seed, 2.0f, 6.0f);

        float chr  = 0.0f;
        float chd  = 0.0f;
        float chm  = 0.0f;

        float mg   = randRange(seed, 0.65f, 0.82f);

        // waveshaper for extra edge, moderate
        float wsa  = randRange(seed, 0.2f, 0.5f);
        int wst    = randInt(seed, 0, 1);

        float eqL  = randRange(seed, -2.0f, 3.0f);
        float eqM  = randRange(seed, 1.0f, 5.0f);
        float eqH  = randRange(seed, 0.0f, 5.0f);
        float eqMF = randRange(seed, 1000.0f, 5000.0f);

        // ring mod occasionally, subtle
        float rmF  = pseudoRand(seed) > 0.7f ? randRange(seed, 100.0f, 500.0f) : 440.0f;
        float rmM  = pseudoRand(seed) > 0.7f ? randRange(seed, 0.1f, 0.3f) : 0.0f;

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         0.5f, 0.0f, 0.0f, 0.3f,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         wsa, wst));
    }
}

inline void generateDarkAmbient(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 1100001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = darkAmbientName(i, seed);

        int o1t   = randInt(seed, 0, 2);
        float o1g = randRange(seed, 0.3f, 0.6f);
        float o1d = randRange(seed, -10.0f, 10.0f);
        int o2t   = randInt(seed, 0, 2);
        float o2g = randRange(seed, 0.2f, 0.5f);
        float o2d = o1d + randRange(seed, -16.0f, 16.0f);  // gentle detune movement
        o2d = clampf(o2d, -22.0f, 22.0f);

        float att  = randRange(seed, 0.2f, 0.8f);
        float dec  = randRange(seed, 0.5f, 2.0f);
        float sus  = randRange(seed, 0.6f, 0.9f);
        float rel  = randRange(seed, 1.5f, 4.0f);

        int dt     = 0;
        float ddr  = randRange(seed, 1.0f, 4.0f);
        float dmx  = randRange(seed, 0.0f, 0.2f);

        float cb   = 16.0f;
        float cr   = 1.0f;

        int ft     = 0;  // dark LP
        float fc   = randRange(seed, 400.0f, 2000.0f);
        float fr   = randRange(seed, 0.3f, 2.0f);
        float fe   = randRange(seed, -0.3f, 0.2f);

        float dlt  = randRange(seed, 200.0f, 700.0f);
        float dlf  = randRange(seed, 0.3f, 0.6f);
        float dlm  = randRange(seed, 0.2f, 0.4f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.6f, 0.95f);
        float rvd  = randRange(seed, 0.2f, 0.5f);
        float rvm  = randRange(seed, 0.3f, 0.5f);
        float rvw  = randRange(seed, 0.7f, 1.0f);

        float ct   = randRange(seed, -40.0f, -15.0f);
        float cra  = randRange(seed, 1.0f, 3.0f);
        float ca   = randRange(seed, 10.0f, 80.0f);
        float crl  = randRange(seed, 100.0f, 400.0f);
        float cm   = randRange(seed, 0.0f, 3.0f);

        float chr  = randRange(seed, 0.2f, 1.5f);
        float chd  = randRange(seed, 0.2f, 0.5f);
        float chm  = randRange(seed, 0.2f, 0.4f);

        float mg   = randRange(seed, 0.5f, 0.7f);

        float phR  = randRange(seed, 0.1f, 0.6f);
        float phD  = randRange(seed, 0.2f, 0.4f);
        float phM  = randRange(seed, 0.1f, 0.3f);
        float phF  = randRange(seed, 0.2f, 0.5f);

        float eqL  = randRange(seed, 0.0f, 4.0f);
        float eqM  = randRange(seed, -4.0f, 0.0f);
        float eqH  = randRange(seed, -6.0f, -1.0f);
        float eqMF = randRange(seed, 400.0f, 1500.0f);

        // subtle ring mod for eeriness
        float rmF  = pseudoRand(seed) > 0.6f ? randRange(seed, 40.0f, 200.0f) : 440.0f;
        float rmM  = pseudoRand(seed) > 0.6f ? randRange(seed, 0.05f, 0.15f) : 0.0f;

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         0.0f, 0));
    }
}

inline void generateExperimental(juce::Array<PresetData>& presets, int count)
{
    uint32_t seed = 1200001u;
    for (int i = 0; i < count; ++i)
    {
        auto nm = experimentalName(i, seed);

        // varied but bounded osc types
        int o1t   = randInt(seed, 0, 4);
        float o1g = randRange(seed, 0.3f, 0.9f);
        float o1d = randRange(seed, -24.0f, 24.0f);
        int o2t   = randInt(seed, 0, 4);
        float o2g = randRange(seed, 0.2f, 0.7f);
        float o2d = randRange(seed, -30.0f, 30.0f);

        float att  = randRange(seed, 0.005f, 0.8f);
        float dec  = randRange(seed, 0.1f, 1.5f);
        float sus  = randRange(seed, 0.2f, 0.8f);
        float rel  = randRange(seed, 0.1f, 3.0f);

        int dt     = randInt(seed, 0, 3);
        float ddr  = randRange(seed, 2.0f, 18.0f);
        float dmx  = clampf(randRange(seed, 0.4f, 0.6f) - (ddr - 2.0f) * 0.012f, 0.25f, 0.6f);

        float cb   = randRange(seed, 6.0f, 16.0f);
        float cr   = randRange(seed, 1.0f, 4.0f);

        int ft     = randInt(seed, 0, 2);
        float fc   = randRange(seed, 300.0f, 9000.0f);
        float fr   = randRange(seed, 0.5f, 4.0f);
        float fe   = randRange(seed, -0.6f, 0.6f);

        float dlt  = randRange(seed, 50.0f, 700.0f);
        float dlf  = randRange(seed, 0.1f, 0.6f);
        float dlm  = randRange(seed, 0.1f, 0.5f);
        int dlp    = randInt(seed, 0, 1);

        float rvs  = randRange(seed, 0.2f, 0.9f);
        float rvd  = randRange(seed, 0.1f, 0.6f);
        float rvm  = randRange(seed, 0.1f, 0.5f);
        float rvw  = randRange(seed, 0.3f, 1.0f);

        float ct   = randRange(seed, -30.0f, -6.0f);
        float cra  = randRange(seed, 1.5f, 8.0f);
        float ca   = randRange(seed, 0.5f, 60.0f);
        float crl  = randRange(seed, 20.0f, 300.0f);
        float cm   = randRange(seed, 0.0f, 6.0f);

        float chr  = randRange(seed, 0.1f, 3.0f);
        float chd  = randRange(seed, 0.1f, 0.6f);
        float chm  = randRange(seed, 0.1f, 0.4f);

        float mg   = randRange(seed, 0.5f, 0.78f);

        float phR  = randRange(seed, 0.1f, 2.0f);
        float phD  = randRange(seed, 0.1f, 0.6f);
        float phM  = randRange(seed, 0.0f, 0.4f);
        float phF  = randRange(seed, 0.1f, 0.6f);

        float eqL  = randRange(seed, -6.0f, 6.0f);
        float eqM  = randRange(seed, -6.0f, 6.0f);
        float eqH  = randRange(seed, -6.0f, 6.0f);
        float eqMF = randRange(seed, 300.0f, 6000.0f);

        float rmF  = pseudoRand(seed) > 0.5f ? randRange(seed, 50.0f, 600.0f) : 440.0f;
        float rmM  = pseudoRand(seed) > 0.5f ? randRange(seed, 0.1f, 0.4f) : 0.0f;

        float wsa  = pseudoRand(seed) > 0.5f ? randRange(seed, 0.2f, 0.6f) : 0.0f;
        int wst    = randInt(seed, 0, 1);

        presets.add(make(nm, o1t, o1g, o1d, o2t, o2g, o2d,
                         att, dec, sus, rel, dt, ddr, dmx, cb, cr,
                         ft, fc, fr, fe,
                         dlt, dlf, dlm, dlp,
                         rvs, rvd, rvm, rvw,
                         ct, cra, ca, crl, cm,
                         chr, chd, chm, mg,
                         phR, phD, phM, phF,
                         eqL, eqM, eqH, eqMF,
                         rmF, rmM,
                         wsa, wst));
    }
}

// ---------------------------------------------------------------------------
// createAll() — builds the complete factory preset bank (540 presets)
// ---------------------------------------------------------------------------
inline juce::Array<PresetData> createAll()
{
    juce::Array<PresetData> presets;
    presets.ensureStorageAllocated(550);

    // ---- Original hand-crafted presets (12) integrated with full params ----
    // These use default values for delay/reverb/comp/chorus and new effects
    presets.add(make("Demolition Kick",
        0, 0.9f, 0.0f,  3, 0.2f, 0.0f,
        0.001f, 0.15f, 0.0f, 0.05f,
        0, 35.0f, 0.9f,  16.0f, 1.0f,
        0, 200.0f, 0.7f, 0.5f,
        10.0f, 0.0f, 0.0f, 0,
        0.05f, 0.5f, 0.0f, 0.2f,
        -10.0f, 4.0f, 0.5f, 30.0f, 4.0f,
        0.0f, 0.0f, 0.0f,
        0.8f,
        0.5f, 0.0f, 0.0f, 0.3f,
        4.0f, -1.0f, -3.0f, 600.0f,
        440.0f, 0.0f,
        0.0f, 0));

    presets.add(make("Industrial Hammer",
        2, 0.8f, 0.0f,  3, 0.3f, 0.0f,
        0.001f, 0.08f, 0.0f, 0.03f,
        0, 28.0f, 0.8f,  6.0f, 3.0f,
        0, 800.0f, 1.5f, 0.3f,
        30.0f, 0.2f, 0.1f, 0,
        0.15f, 0.4f, 0.05f, 0.3f,
        -8.0f, 6.0f, 0.3f, 25.0f, 5.0f,
        0.0f, 0.0f, 0.0f,
        0.7f,
        0.5f, 0.0f, 0.0f, 0.3f,
        2.0f, 3.0f, -1.0f, 1200.0f,
        300.0f, 0.3f,
        0.4f, 2));

    presets.add(make("Acid Scream",
        1, 0.8f, 0.0f,  1, 0.3f, 7.0f,
        0.001f, 0.20f, 0.6f, 0.15f,
        1, 18.0f, 0.6f,  16.0f, 1.0f,
        0, 800.0f, 8.0f, 0.7f,
        120.0f, 0.3f, 0.1f, 0,
        0.1f, 0.4f, 0.05f, 0.3f,
        -10.0f, 3.0f, 2.0f, 50.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.7f,
        1.5f, 0.5f, 0.3f, 0.5f,
        1.0f, 3.0f, 2.0f, 2000.0f,
        440.0f, 0.0f,
        0.2f, 0));

    presets.add(make("Concrete Texture",
        3, 0.6f, 0.0f,  1, 0.3f, 0.0f,
        0.300f, 0.40f, 0.6f, 1.00f,
        1, 10.0f, 0.4f,  8.0f, 4.0f,
        0, 3000.0f, 1.0f, 0.2f,
        250.0f, 0.5f, 0.3f, 1,
        0.5f, 0.3f, 0.2f, 0.7f,
        -20.0f, 2.0f, 15.0f, 120.0f, 1.0f,
        0.8f, 0.3f, 0.2f,
        0.6f,
        0.3f, 0.3f, 0.2f, 0.4f,
        1.0f, -1.0f, -2.0f, 800.0f,
        440.0f, 0.0f,
        0.0f, 0));

    presets.add(make("Razor Lead",
        1, 0.7f, 0.0f,  1, 0.6f, 15.0f,
        0.001f, 0.10f, 0.8f, 0.10f,
        0, 22.0f, 0.7f,  16.0f, 1.0f,
        0, 5000.0f, 2.0f, 0.3f,
        80.0f, 0.25f, 0.15f, 1,
        0.15f, 0.3f, 0.08f, 0.5f,
        -12.0f, 4.0f, 3.0f, 40.0f, 4.0f,
        1.0f, 0.15f, 0.1f,
        0.7f,
        0.8f, 0.3f, 0.2f, 0.4f,
        -1.0f, 2.0f, 3.0f, 3000.0f,
        440.0f, 0.0f,
        0.3f, 0));

    presets.add(make("Bit Rot",
        2, 0.7f, 0.0f,  3, 0.2f, 0.0f,
        0.001f, 0.15f, 0.5f, 0.20f,
        0, 15.0f, 0.5f,  4.0f, 6.0f,
        0, 6000.0f, 1.0f, 0.0f,
        60.0f, 0.3f, 0.1f, 0,
        0.2f, 0.4f, 0.1f, 0.4f,
        -15.0f, 5.0f, 1.0f, 35.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.6f,
        0.5f, 0.0f, 0.0f, 0.3f,
        0.0f, 0.0f, 0.0f, 1000.0f,
        440.0f, 0.0f,
        0.0f, 0));

    presets.add(make("Warehouse Stab",
        1, 0.7f, 0.0f,  2, 0.5f, 0.0f,
        0.001f, 0.20f, 0.3f, 0.10f,
        1, 18.0f, 0.5f,  16.0f, 1.0f,
        0, 3000.0f, 1.5f, 0.4f,
        100.0f, 0.3f, 0.15f, 1,
        0.1f, 0.5f, 0.08f, 0.4f,
        -10.0f, 5.0f, 0.5f, 25.0f, 4.0f,
        0.0f, 0.0f, 0.0f,
        0.7f,
        0.5f, 0.0f, 0.0f, 0.3f,
        0.0f, 2.0f, 1.0f, 1500.0f,
        440.0f, 0.0f,
        0.0f, 0));

    presets.add(make("Punishment Bass",
        0, 0.9f, 0.0f,  1, 0.4f, 0.0f,
        0.005f, 0.15f, 0.7f, 0.15f,
        0, 30.0f, 0.7f,  16.0f, 1.0f,
        0, 400.0f, 2.0f, 0.2f,
        10.0f, 0.0f, 0.0f, 0,
        0.05f, 0.5f, 0.0f, 0.2f,
        -15.0f, 5.0f, 2.0f, 60.0f, 4.0f,
        0.0f, 0.0f, 0.0f,
        0.8f,
        0.5f, 0.0f, 0.0f, 0.3f,
        3.0f, 0.0f, -3.0f, 500.0f,
        440.0f, 0.0f,
        0.2f, 0));

    presets.add(make("Broken Machine",
        2, 0.6f, 0.0f,  3, 0.5f, 0.0f,
        0.001f, 0.10f, 0.4f, 0.08f,
        5, 25.0f, 0.7f,  3.0f, 8.0f,
        0, 2000.0f, 3.0f, 0.5f,
        70.0f, 0.4f, 0.2f, 0,
        0.2f, 0.3f, 0.1f, 0.4f,
        -8.0f, 8.0f, 0.3f, 20.0f, 6.0f,
        0.0f, 0.0f, 0.0f,
        0.5f,
        0.5f, 0.0f, 0.0f, 0.3f,
        0.0f, 4.0f, 2.0f, 2000.0f,
        600.0f, 0.4f,
        0.5f, 1));

    presets.add(make("Feedback Loop",
        3, 0.3f, 0.0f,  0, 0.2f, 0.0f,
        0.050f, 0.30f, 0.5f, 0.50f,
        1, 25.0f, 0.5f,  16.0f, 1.0f,
        2, 1200.0f, 15.0f, 0.6f,
        300.0f, 0.7f, 0.4f, 1,
        0.4f, 0.2f, 0.2f, 0.8f,
        -18.0f, 3.0f, 5.0f, 80.0f, 2.0f,
        0.5f, 0.3f, 0.2f,
        0.6f,
        0.5f, 0.4f, 0.3f, 0.5f,
        0.0f, -2.0f, -3.0f, 900.0f,
        440.0f, 0.0f,
        0.0f, 0));

    presets.add(make("Piledriver",
        1, 0.7f, -7.0f,  1, 0.7f, 7.0f,
        0.001f, 0.10f, 0.7f, 0.10f,
        0, 35.0f, 0.8f,  16.0f, 1.0f,
        0, 8000.0f, 1.5f, 0.2f,
        50.0f, 0.2f, 0.1f, 0,
        0.1f, 0.3f, 0.05f, 0.4f,
        -8.0f, 6.0f, 1.0f, 30.0f, 5.0f,
        0.0f, 0.0f, 0.0f,
        0.7f,
        0.5f, 0.0f, 0.0f, 0.3f,
        -1.0f, 3.0f, 4.0f, 4000.0f,
        440.0f, 0.0f,
        0.3f, 0));

    presets.add(make("Sample Mangler",
        1, 0.3f, 0.0f,  3, 0.2f, 0.0f,
        0.001f, 0.20f, 0.6f, 0.20f,
        5, 15.0f, 0.5f,  10.0f, 3.0f,
        0, 8000.0f, 2.0f, 0.3f,
        150.0f, 0.45f, 0.2f, 1,
        0.3f, 0.4f, 0.15f, 0.5f,
        -12.0f, 4.0f, 2.0f, 45.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.6f,
        0.5f, 0.0f, 0.0f, 0.3f,
        0.0f, 0.0f, 0.0f, 1000.0f,
        800.0f, 0.2f,
        0.3f, 2));

    // ---- Procedurally generated categories ----
    int before = presets.size();
    generateKicks(presets, 50);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Kicks";

    before = presets.size();
    generateBass(presets, 60);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Bass";

    before = presets.size();
    generateLeads(presets, 60);
    for (int i = before; i < presets.size(); ++i)
    {
        presets.getReference(i).category = "Leads";
        // Supersaw leads: 3-7 unison voices with musical detune
        presets.getReference(i).state.setProperty("unisonVoices", 3 + (i % 5), nullptr);
        presets.getReference(i).state.setProperty("unisonDetune", 12.0f + (i % 6) * 2.5f, nullptr);
    }

    before = presets.size();
    generateStabs(presets, 50);
    for (int i = before; i < presets.size(); ++i)
    {
        presets.getReference(i).category = "Stabs";
        // Fat detuned stabs: 2-5 voices
        presets.getReference(i).state.setProperty("unisonVoices", 2 + (i % 4), nullptr);
        presets.getReference(i).state.setProperty("unisonDetune", 10.0f + (i % 5) * 2.0f, nullptr);
    }

    before = presets.size();
    generatePads(presets, 40);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Pads";

    before = presets.size();
    generateAcid(presets, 50);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Acid";

    before = presets.size();
    generateIndustrial(presets, 50);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Industrial";

    before = presets.size();
    generateFX(presets, 40);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "FX";

    before = presets.size();
    generatePercussion(presets, 40);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Percussion";

    before = presets.size();
    generateScream(presets, 40);
    for (int i = before; i < presets.size(); ++i)
    {
        presets.getReference(i).category = "Scream";
        // Hoover/rave screams: thick 4-7 voice supersaw with wide detune
        presets.getReference(i).state.setProperty("unisonVoices", 4 + (i % 4), nullptr);
        presets.getReference(i).state.setProperty("unisonDetune", 18.0f + (i % 5) * 2.5f, nullptr);
    }

    before = presets.size();
    generateDarkAmbient(presets, 30);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Dark Ambient";

    before = presets.size();
    generateExperimental(presets, 30);
    for (int i = before; i < presets.size(); ++i) presets.getReference(i).category = "Experimental";

    // Tag the original 12 hand-crafted presets by name
    auto tagPreset = [&](const juce::String& name, const juce::String& cat) {
        for (int i = 0; i < presets.size(); ++i)
            if (presets.getReference(i).name == name)
                presets.getReference(i).category = cat;
    };
    tagPreset("Demolition Kick", "Kicks");
    tagPreset("Industrial Hammer", "Kicks");
    tagPreset("Piledriver", "Kicks");
    tagPreset("Acid Scream", "Acid");
    tagPreset("Razor Lead", "Leads");
    tagPreset("Punishment Bass", "Bass");
    tagPreset("Warehouse Stab", "Stabs");
    tagPreset("Concrete Texture", "FX");
    tagPreset("Bit Rot", "Industrial");
    tagPreset("Broken Machine", "Industrial");
    tagPreset("Feedback Loop", "FX");
    tagPreset("Sample Mangler", "FX");

    return presets;
}

} // namespace FactoryPresets
