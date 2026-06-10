#include "PluginProcessor.h"
#include "PluginEditor.h"

SchranzMachineProcessor::SchranzMachineProcessor()
    : AudioProcessor(BusesProperties()
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout()),
      presetManager(apvts)
{
}

SchranzMachineProcessor::~SchranzMachineProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout SchranzMachineProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillator 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc1Type", "Osc 1 Type",
        juce::StringArray{"Sine", "Saw", "Square", "Noise", "Sample"}, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Gain", "Osc 1 Gain", 0.0f, 1.0f, 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Detune", "Osc 1 Detune", -100.0f, 100.0f, 0.0f));

    // Oscillator 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc2Type", "Osc 2 Type",
        juce::StringArray{"Sine", "Saw", "Square", "Noise", "Sample"}, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc2Gain", "Osc 2 Gain", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc2Detune", "Osc 2 Detune", -100.0f, 100.0f, 0.0f));

    // Unison (applies to both oscillators — supersaw character)
    params.push_back(std::make_unique<juce::AudioParameterInt>("unisonVoices", "Unison", 1, 8, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("unisonDetune", "Unison Detune",
        juce::NormalisableRange<float>(0.0f, 50.0f, 0.1f), 15.0f));

    // Amp Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ampAttack", "Attack",
        juce::NormalisableRange<float>(0.001f, 2.0f, 0.001f, 0.3f), 0.005f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ampDecay", "Decay",
        juce::NormalisableRange<float>(0.001f, 2.0f, 0.001f, 0.3f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ampSustain", "Sustain", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ampRelease", "Release",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.2f));

    // Distortion
    params.push_back(std::make_unique<juce::AudioParameterChoice>("distType", "Dist Type",
        juce::StringArray{"Hard Clip", "Soft Clip", "Foldback", "Bit Crush", "Rectify", "Scream"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distDrive", "Drive",
        juce::NormalisableRange<float>(1.0f, 50.0f, 0.1f, 0.4f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distMix", "Dist Mix", 0.0f, 1.0f, 0.5f));

    // Bit Crusher
    params.push_back(std::make_unique<juce::AudioParameterFloat>("crushBits", "Bit Depth",
        juce::NormalisableRange<float>(2.0f, 16.0f, 0.1f), 16.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("crushRate", "Downsample",
        juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f), 1.0f));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("filterType", "Filter Type",
        juce::StringArray{"Low Pass", "High Pass", "Band Pass", "Notch"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterCutoff", "Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 20000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterResonance", "Resonance",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.3f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterEnvAmount", "Filter Env",
        -1.0f, 1.0f, 0.0f));

    // Delay
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time",
        juce::NormalisableRange<float>(10.0f, 1000.0f, 1.0f, 0.4f), 250.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayFeedback", "Delay FB", 0.0f, 0.85f, 0.4f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayMix", "Delay Mix", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("delayPingPong", "Ping Pong", false));

    // Reverb
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverbSize", "Room Size", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverbDamping", "Damping", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverbMix", "Reverb Mix", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverbWidth", "Width", 0.0f, 1.0f, 1.0f));

    // Compressor
    params.push_back(std::make_unique<juce::AudioParameterFloat>("compThreshold", "Comp Thresh",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("compRatio", "Comp Ratio",
        juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f, 0.5f), 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("compAttack", "Comp Attack",
        juce::NormalisableRange<float>(0.1f, 100.0f, 0.1f, 0.4f), 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("compRelease", "Comp Release",
        juce::NormalisableRange<float>(10.0f, 500.0f, 1.0f, 0.4f), 50.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("compMakeup", "Comp Makeup",
        juce::NormalisableRange<float>(0.0f, 12.0f, 0.1f), 0.0f));

    // Chorus
    params.push_back(std::make_unique<juce::AudioParameterFloat>("chorusRate", "Chorus Rate",
        juce::NormalisableRange<float>(0.1f, 5.0f, 0.01f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("chorusDepth", "Chorus Depth", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("chorusMix", "Chorus Mix", 0.0f, 1.0f, 0.0f));

    // Master
    params.push_back(std::make_unique<juce::AudioParameterFloat>("masterGain", "Master",
        0.0f, 1.0f, 0.7f));

    // Phaser
    params.push_back(std::make_unique<juce::AudioParameterFloat>("phaserRate", "Phaser Rate",
        juce::NormalisableRange<float>(0.1f, 5.0f, 0.01f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("phaserDepth", "Phaser Depth", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("phaserMix", "Phaser Mix", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("phaserFeedback", "Phaser FB",
        juce::NormalisableRange<float>(0.0f, 0.7f, 0.01f), 0.3f));

    // EQ
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eqLowGain", "EQ Low",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eqMidGain", "EQ Mid",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eqHighGain", "EQ High",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eqMidFreq", "EQ Mid Freq",
        juce::NormalisableRange<float>(200.0f, 8000.0f, 1.0f, 0.3f), 1000.0f));

    // Ring Modulator
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ringModFreq", "Ring Mod Freq",
        juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f, 0.3f), 440.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ringModMix", "Ring Mod Mix", 0.0f, 1.0f, 0.0f));

    // Waveshaper
    params.push_back(std::make_unique<juce::AudioParameterFloat>("wsAmount", "WS Amount", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("wsType", "WS Type",
        juce::StringArray{"Tanh", "Sin", "Abs", "Cubic"}, 0));

    // Sample controls
    params.push_back(std::make_unique<juce::AudioParameterBool>("sampleLoop", "Sample Loop", false));
    params.push_back(std::make_unique<juce::AudioParameterInt>("sampleRootNote", "Root Note", 0, 127, 60));

    return { params.begin(), params.end() };
}

void SchranzMachineProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    schranzSynth.prepare(sampleRate, samplesPerBlock);
    delayEngine.prepare(sampleRate, samplesPerBlock);
    reverbEngine.prepare(sampleRate);
    compressorL.prepare(sampleRate);
    compressorR.prepare(sampleRate);
    chorusEngine.prepare(sampleRate);
    phaserEngine.prepare(sampleRate);
    eqEngineL.prepare(sampleRate);
    eqEngineR.prepare(sampleRate);
    ringModEngine.prepare(sampleRate);
}

void SchranzMachineProcessor::releaseResources() {}

bool SchranzMachineProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void SchranzMachineProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    // MIDI pattern playback
    if (patternPlaying.load() && patternBank.getNumPatterns() > 0)
    {
        int idx = juce::jlimit(0, patternBank.getNumPatterns() - 1, currentPatternIndex.load());
        const auto& pattern = patternBank.getPattern(idx);
        double bpm = juce::jmax(40.0f, patternBpm.load());
        double samplesPerBeat = (60.0 / bpm) * getSampleRate();
        double patternLengthSamples = pattern.lengthInBeats * samplesPerBeat;
        int numSamples = buffer.getNumSamples();

        if (patternNotesTriggered.size() != pattern.notes.size())
            patternNotesTriggered.assign(pattern.notes.size(), false);

        for (int s = 0; s < numSamples; ++s)
        {
            double currentBeat = patternPositionSamples / samplesPerBeat;

            for (size_t n = 0; n < pattern.notes.size(); ++n)
            {
                if (!patternNotesTriggered[n] && currentBeat >= pattern.notes[n].startBeat)
                {
                    int note = juce::jlimit(0, 127, pattern.notes[n].noteNumber);
                    int vel = juce::jlimit(1, 127, pattern.notes[n].velocity);
                    midiMessages.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8)vel), s);
                    activePatternNotes.push_back(note);
                    patternNotesTriggered[n] = true;
                }
                if (patternNotesTriggered[n])
                {
                    double endBeat = pattern.notes[n].startBeat + pattern.notes[n].duration;
                    if (currentBeat >= endBeat)
                    {
                        int note = juce::jlimit(0, 127, pattern.notes[n].noteNumber);
                        for (auto it = activePatternNotes.begin(); it != activePatternNotes.end(); ++it)
                        {
                            if (*it == note)
                            {
                                midiMessages.addEvent(juce::MidiMessage::noteOff(1, note), s);
                                activePatternNotes.erase(it);
                                break;
                            }
                        }
                    }
                }
            }

            patternPositionSamples += 1.0;
            if (patternPositionSamples >= patternLengthSamples)
            {
                patternPositionSamples = 0.0;
                for (int activeNote : activePatternNotes)
                    midiMessages.addEvent(juce::MidiMessage::noteOff(1, activeNote), s);
                activePatternNotes.clear();
                std::fill(patternNotesTriggered.begin(), patternNotesTriggered.end(), false);
            }
        }
    }

    updateVoiceParameters();
    updateEffectParameters();
    schranzSynth.processBlock(buffer, midiMessages);

    int numSamples = buffer.getNumSamples();
    bool isStereo = buffer.getNumChannels() >= 2;

    for (int i = 0; i < numSamples; ++i)
    {
        float l = buffer.getSample(0, i);
        float r = isStereo ? buffer.getSample(1, i) : l;

        eqEngineL.process(l);
        eqEngineR.process(r);

        waveshaperL.process(l);
        waveshaperR.process(r);

        ringModEngine.process(l, r);

        l = compressorL.process(l);
        r = compressorR.process(r);

        phaserEngine.process(l, r);
        chorusEngine.process(l, r);
        delayEngine.process(l, r);
        reverbEngine.process(l, r);

        // NaN/Inf safety + soft limiter to avoid hard-clip noise
        if (!std::isfinite(l)) l = 0.0f;
        if (!std::isfinite(r)) r = 0.0f;
        l = juce::jlimit(-3.0f, 3.0f, l);
        r = juce::jlimit(-3.0f, 3.0f, r);

        buffer.setSample(0, i, l);
        if (isStereo) buffer.setSample(1, i, r);
    }

    float master = apvts.getRawParameterValue("masterGain")->load();
    buffer.applyGain(master);

    // Soft limiter at output stage (tanh-based, prevents harsh clipping)
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i)
        {
            if (!std::isfinite(data[i])) { data[i] = 0.0f; continue; }
            float x = data[i];
            // Soft knee compression above 0.85, prevents harsh clipping
            if (std::abs(x) > 0.85f)
            {
                float sign = x > 0 ? 1.0f : -1.0f;
                float over = std::abs(x) - 0.85f;
                x = sign * (0.85f + 0.15f * std::tanh(over / 0.15f));
            }
            data[i] = x;
        }
    }
}

void SchranzMachineProcessor::updateVoiceParameters()
{
    int osc1Type = static_cast<int>(apvts.getRawParameterValue("osc1Type")->load());
    float osc1Gain = apvts.getRawParameterValue("osc1Gain")->load();
    float osc1Detune = apvts.getRawParameterValue("osc1Detune")->load();

    int osc2Type = static_cast<int>(apvts.getRawParameterValue("osc2Type")->load());
    float osc2Gain = apvts.getRawParameterValue("osc2Gain")->load();
    float osc2Detune = apvts.getRawParameterValue("osc2Detune")->load();

    int unisonVoices = static_cast<int>(apvts.getRawParameterValue("unisonVoices")->load());
    float unisonDetune = apvts.getRawParameterValue("unisonDetune")->load();

    float attack = apvts.getRawParameterValue("ampAttack")->load();
    float decay = apvts.getRawParameterValue("ampDecay")->load();
    float sustain = apvts.getRawParameterValue("ampSustain")->load();
    float release = apvts.getRawParameterValue("ampRelease")->load();

    int distType = static_cast<int>(apvts.getRawParameterValue("distType")->load());
    float distDrive = apvts.getRawParameterValue("distDrive")->load();
    float distMix = apvts.getRawParameterValue("distMix")->load();

    float crushBits = apvts.getRawParameterValue("crushBits")->load();
    float crushRate = apvts.getRawParameterValue("crushRate")->load();

    int filterType = static_cast<int>(apvts.getRawParameterValue("filterType")->load());
    float filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
    float filterRes = apvts.getRawParameterValue("filterResonance")->load();
    float filterEnv = apvts.getRawParameterValue("filterEnvAmount")->load();

    bool sampleLoop = apvts.getRawParameterValue("sampleLoop")->load() > 0.5f;
    int sampleRoot = static_cast<int>(apvts.getRawParameterValue("sampleRootNote")->load());

    for (int i = 0; i < schranzSynth.getSynth().getNumVoices(); ++i)
    {
        if (auto* voice = schranzSynth.getVoice(i))
        {
            voice->getOsc1().setType(static_cast<OscillatorType>(osc1Type));
            voice->setOsc1Gain(osc1Gain);
            voice->getOsc1().setDetune(osc1Detune);
            voice->getOsc1().setUnisonCount(unisonVoices);
            voice->getOsc1().setUnisonDetune(unisonDetune);

            voice->getOsc2().setType(static_cast<OscillatorType>(osc2Type));
            voice->setOsc2Gain(osc2Gain);
            voice->getOsc2().setDetune(osc2Detune);
            voice->getOsc2().setUnisonCount(unisonVoices);
            voice->getOsc2().setUnisonDetune(unisonDetune);

            voice->setAmpAttack(attack);
            voice->setAmpDecay(decay);
            voice->setAmpSustain(sustain);
            voice->setAmpRelease(release);

            voice->getDistortion().setType(static_cast<DistortionType>(distType));
            voice->getDistortion().setDrive(distDrive);
            voice->getDistortion().setMix(distMix);
            voice->getDistortion().setBitDepth(crushBits);
            voice->getDistortion().setDownsampleRate(crushRate);

            voice->getFilter().setType(static_cast<FilterType>(filterType));
            voice->getFilter().setCutoff(filterCutoff);
            voice->getFilter().setResonance(filterRes);
            voice->getFilter().setEnvelopeAmount(filterEnv);

            voice->getSampleEngine().setLoopEnabled(sampleLoop);
            voice->getSampleEngine().setRootNote(sampleRoot);
        }
    }
}

void SchranzMachineProcessor::updateEffectParameters()
{
    delayEngine.setTime(apvts.getRawParameterValue("delayTime")->load());
    delayEngine.setFeedback(apvts.getRawParameterValue("delayFeedback")->load());
    delayEngine.setMix(apvts.getRawParameterValue("delayMix")->load());
    delayEngine.setPingPong(apvts.getRawParameterValue("delayPingPong")->load() > 0.5f);

    reverbEngine.setRoomSize(apvts.getRawParameterValue("reverbSize")->load());
    reverbEngine.setDamping(apvts.getRawParameterValue("reverbDamping")->load());
    reverbEngine.setMix(apvts.getRawParameterValue("reverbMix")->load());
    reverbEngine.setWidth(apvts.getRawParameterValue("reverbWidth")->load());

    compressorL.setThreshold(apvts.getRawParameterValue("compThreshold")->load());
    compressorL.setRatio(apvts.getRawParameterValue("compRatio")->load());
    compressorL.setAttack(apvts.getRawParameterValue("compAttack")->load());
    compressorL.setRelease(apvts.getRawParameterValue("compRelease")->load());
    compressorL.setMakeupGain(apvts.getRawParameterValue("compMakeup")->load());
    compressorR.setThreshold(apvts.getRawParameterValue("compThreshold")->load());
    compressorR.setRatio(apvts.getRawParameterValue("compRatio")->load());
    compressorR.setAttack(apvts.getRawParameterValue("compAttack")->load());
    compressorR.setRelease(apvts.getRawParameterValue("compRelease")->load());
    compressorR.setMakeupGain(apvts.getRawParameterValue("compMakeup")->load());

    chorusEngine.setRate(apvts.getRawParameterValue("chorusRate")->load());
    chorusEngine.setDepth(apvts.getRawParameterValue("chorusDepth")->load());
    chorusEngine.setMix(apvts.getRawParameterValue("chorusMix")->load());

    phaserEngine.setRate(apvts.getRawParameterValue("phaserRate")->load());
    phaserEngine.setDepth(apvts.getRawParameterValue("phaserDepth")->load());
    phaserEngine.setMix(apvts.getRawParameterValue("phaserMix")->load());
    phaserEngine.setFeedback(apvts.getRawParameterValue("phaserFeedback")->load());

    eqEngineL.setLowGain(apvts.getRawParameterValue("eqLowGain")->load());
    eqEngineL.setMidGain(apvts.getRawParameterValue("eqMidGain")->load());
    eqEngineL.setHighGain(apvts.getRawParameterValue("eqHighGain")->load());
    eqEngineL.setMidFreq(apvts.getRawParameterValue("eqMidFreq")->load());
    eqEngineR.setLowGain(apvts.getRawParameterValue("eqLowGain")->load());
    eqEngineR.setMidGain(apvts.getRawParameterValue("eqMidGain")->load());
    eqEngineR.setHighGain(apvts.getRawParameterValue("eqHighGain")->load());
    eqEngineR.setMidFreq(apvts.getRawParameterValue("eqMidFreq")->load());

    ringModEngine.setFrequency(apvts.getRawParameterValue("ringModFreq")->load());
    ringModEngine.setMix(apvts.getRawParameterValue("ringModMix")->load());

    waveshaperL.setAmount(apvts.getRawParameterValue("wsAmount")->load());
    waveshaperL.setType(static_cast<int>(apvts.getRawParameterValue("wsType")->load()));
    waveshaperR.setAmount(apvts.getRawParameterValue("wsAmount")->load());
    waveshaperR.setType(static_cast<int>(apvts.getRawParameterValue("wsType")->load()));
}

void SchranzMachineProcessor::loadSample(const juce::File& file)
{
    if (schranzSynth.loadSampleToAllVoices(file))
    {
        loadedSampleName = file.getFileNameWithoutExtension();
        loadedSamplePath = file.getFullPathName();
    }
}

void SchranzMachineProcessor::startPatternPlayback(int patternIndex)
{
    if (patternIndex < 0 || patternIndex >= patternBank.getNumPatterns()) return;
    currentPatternIndex.store(patternIndex);
    patternPositionSamples = 0.0;
    patternNotesTriggered.assign(patternBank.getPattern(patternIndex).notes.size(), false);
    activePatternNotes.clear();
    patternPlaying.store(true);
}

void SchranzMachineProcessor::stopPatternPlayback()
{
    patternPlaying.store(false);
    // Note-offs will be sent on next processBlock via keyboardState? No, we need explicit cleanup.
    // Send note-offs directly through keyboard state for active notes
    for (int note : activePatternNotes)
        keyboardState.noteOff(1, note, 0.0f);
    activePatternNotes.clear();
}

juce::AudioProcessorEditor* SchranzMachineProcessor::createEditor()
{
    return new SchranzMachineEditor(*this);
}

int SchranzMachineProcessor::getNumPrograms() { return presetManager.getNumPresets(); }
int SchranzMachineProcessor::getCurrentProgram() { return presetManager.getCurrentPresetIndex(); }
void SchranzMachineProcessor::setCurrentProgram(int index) { presetManager.loadPreset(index); }
const juce::String SchranzMachineProcessor::getProgramName(int index) { return presetManager.getPresetName(index); }
void SchranzMachineProcessor::changeProgramName(int, const juce::String&) {}

void SchranzMachineProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty("samplePath", loadedSamplePath, nullptr);
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SchranzMachineProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xml);
        if (state.isValid())
        {
            apvts.replaceState(state);
            auto samplePath = state.getProperty("samplePath").toString();
            if (samplePath.isNotEmpty())
            {
                juce::File sampleFile(samplePath);
                if (sampleFile.existsAsFile())
                    loadSample(sampleFile);
            }
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SchranzMachineProcessor();
}
