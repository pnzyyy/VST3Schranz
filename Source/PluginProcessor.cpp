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
        juce::NormalisableRange<float>(0.1f, 20.0f, 0.01f, 0.3f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterEnvAmount", "Filter Env",
        -1.0f, 1.0f, 0.0f));

    // Master
    params.push_back(std::make_unique<juce::AudioParameterFloat>("masterGain", "Master",
        0.0f, 1.0f, 0.7f));

    // Sample controls
    params.push_back(std::make_unique<juce::AudioParameterBool>("sampleLoop", "Sample Loop", false));
    params.push_back(std::make_unique<juce::AudioParameterInt>("sampleRootNote", "Root Note", 0, 127, 60));

    return { params.begin(), params.end() };
}

void SchranzMachineProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    schranzSynth.prepare(sampleRate, samplesPerBlock);
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

    updateVoiceParameters();
    schranzSynth.processBlock(buffer, midiMessages);

    float master = apvts.getRawParameterValue("masterGain")->load();
    buffer.applyGain(master);
}

void SchranzMachineProcessor::updateVoiceParameters()
{
    int osc1Type = static_cast<int>(apvts.getRawParameterValue("osc1Type")->load());
    float osc1Gain = apvts.getRawParameterValue("osc1Gain")->load();
    float osc1Detune = apvts.getRawParameterValue("osc1Detune")->load();

    int osc2Type = static_cast<int>(apvts.getRawParameterValue("osc2Type")->load());
    float osc2Gain = apvts.getRawParameterValue("osc2Gain")->load();
    float osc2Detune = apvts.getRawParameterValue("osc2Detune")->load();

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

            voice->getOsc2().setType(static_cast<OscillatorType>(osc2Type));
            voice->setOsc2Gain(osc2Gain);
            voice->getOsc2().setDetune(osc2Detune);

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

void SchranzMachineProcessor::loadSample(const juce::File& file)
{
    if (schranzSynth.loadSampleToAllVoices(file))
    {
        loadedSampleName = file.getFileNameWithoutExtension();
        loadedSamplePath = file.getFullPathName();
    }
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
