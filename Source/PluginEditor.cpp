#include "PluginEditor.h"

// -- DragDropArea --
DragDropArea::DragDropArea(SchranzMachineProcessor& p) : processor(p) {}

void DragDropArea::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(isDragOver ? juce::Colour(0xFF330000) : juce::Colour(0xFF1A1A1A));
    g.fillRoundedRectangle(bounds, 6.0f);
    g.setColour(juce::Colour(0xFF444444));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 6.0f, 1.0f);

    auto sampleName = processor.getLoadedSampleName();
    g.setColour(juce::Colour(0xFF888888));
    g.setFont(14.0f);
    if (sampleName.isEmpty())
        g.drawText("Drop audio file here (WAV, AIFF, FLAC, MP3)", bounds, juce::Justification::centred);
    else
        g.drawText("Sample: " + sampleName, bounds, juce::Justification::centred);
}

bool DragDropArea::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& f : files)
    {
        auto ext = juce::File(f).getFileExtension().toLowerCase();
        if (ext == ".wav" || ext == ".aif" || ext == ".aiff" ||
            ext == ".flac" || ext == ".mp3" || ext == ".ogg")
            return true;
    }
    return false;
}

void DragDropArea::filesDropped(const juce::StringArray& files, int, int)
{
    isDragOver = false;
    if (!files.isEmpty())
    {
        processor.loadSample(juce::File(files[0]));
        repaint();
    }
}

void DragDropArea::fileDragEnter(const juce::StringArray&, int, int)
{
    isDragOver = true;
    repaint();
}

void DragDropArea::fileDragExit(const juce::StringArray&)
{
    isDragOver = false;
    repaint();
}

// -- Editor --
SchranzMachineEditor::SchranzMachineEditor(SchranzMachineProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), dragDropArea(p)
{
    setLookAndFeel(&schranzLnf);
    setSize(780, 580);

    // Preset bar
    addAndMakeVisible(prevPresetBtn);
    addAndMakeVisible(nextPresetBtn);
    addAndMakeVisible(presetNameLabel);
    presetNameLabel.setJustificationType(juce::Justification::centred);
    presetNameLabel.setFont(juce::Font(16.0f).boldened());
    presetNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFCC0000));
    updatePresetLabel();

    prevPresetBtn.onClick = [this] { processorRef.getPresetManager().previousPreset(); updatePresetLabel(); };
    nextPresetBtn.onClick = [this] { processorRef.getPresetManager().nextPreset(); updatePresetLabel(); };

    // Osc 1
    addAndMakeVisible(osc1Label);
    setupLabel(osc1Label);
    addAndMakeVisible(osc1TypeBox);
    osc1TypeBox.addItemList({"Sine", "Saw", "Square", "Noise", "Sample"}, 1);
    osc1TypeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), "osc1Type", osc1TypeBox);
    addAndMakeVisible(osc1GainSlider);
    setupSlider(osc1GainSlider);
    osc1GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), "osc1Gain", osc1GainSlider);
    addAndMakeVisible(osc1DetuneSlider);
    setupSlider(osc1DetuneSlider);
    osc1DetuneAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), "osc1Detune", osc1DetuneSlider);

    // Osc 2
    addAndMakeVisible(osc2Label);
    setupLabel(osc2Label);
    addAndMakeVisible(osc2TypeBox);
    osc2TypeBox.addItemList({"Sine", "Saw", "Square", "Noise", "Sample"}, 1);
    osc2TypeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), "osc2Type", osc2TypeBox);
    addAndMakeVisible(osc2GainSlider);
    setupSlider(osc2GainSlider);
    osc2GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), "osc2Gain", osc2GainSlider);
    addAndMakeVisible(osc2DetuneSlider);
    setupSlider(osc2DetuneSlider);
    osc2DetuneAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getAPVTS(), "osc2Detune", osc2DetuneSlider);

    // Envelope
    addAndMakeVisible(envLabel);
    setupLabel(envLabel);
    addAndMakeVisible(attackSlider);   setupSlider(attackSlider);
    addAndMakeVisible(decaySlider);    setupSlider(decaySlider);
    addAndMakeVisible(sustainSlider);  setupSlider(sustainSlider);
    addAndMakeVisible(releaseSlider);  setupSlider(releaseSlider);
    attackAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "ampAttack", attackSlider);
    decayAttach   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "ampDecay", decaySlider);
    sustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "ampSustain", sustainSlider);
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "ampRelease", releaseSlider);

    // Distortion
    addAndMakeVisible(distLabel);
    setupLabel(distLabel);
    addAndMakeVisible(distTypeBox);
    distTypeBox.addItemList({"Hard Clip", "Soft Clip", "Foldback", "Bit Crush", "Rectify", "Scream"}, 1);
    distTypeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), "distType", distTypeBox);
    addAndMakeVisible(driveSlider);   setupSlider(driveSlider);
    addAndMakeVisible(distMixSlider); setupSlider(distMixSlider);
    driveAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "distDrive", driveSlider);
    distMixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "distMix", distMixSlider);

    // Crusher
    addAndMakeVisible(crushLabel);
    setupLabel(crushLabel);
    addAndMakeVisible(crushBitsSlider); setupSlider(crushBitsSlider);
    addAndMakeVisible(crushRateSlider); setupSlider(crushRateSlider);
    crushBitsAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "crushBits", crushBitsSlider);
    crushRateAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "crushRate", crushRateSlider);

    // Filter
    addAndMakeVisible(filterLabel);
    setupLabel(filterLabel);
    addAndMakeVisible(filterTypeBox);
    filterTypeBox.addItemList({"Low Pass", "High Pass", "Band Pass", "Notch"}, 1);
    filterTypeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.getAPVTS(), "filterType", filterTypeBox);
    addAndMakeVisible(cutoffSlider);    setupSlider(cutoffSlider);
    addAndMakeVisible(resonanceSlider); setupSlider(resonanceSlider);
    addAndMakeVisible(filterEnvSlider); setupSlider(filterEnvSlider);
    cutoffAttach    = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "filterCutoff", cutoffSlider);
    resonanceAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "filterResonance", resonanceSlider);
    filterEnvAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "filterEnvAmount", filterEnvSlider);

    // Master
    addAndMakeVisible(masterLabel);
    setupLabel(masterLabel);
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    masterSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    masterAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getAPVTS(), "masterGain", masterSlider);

    // Drag drop
    addAndMakeVisible(dragDropArea);

    // Sample loop
    addAndMakeVisible(sampleLoopBtn);
    sampleLoopBtn.setColour(juce::ToggleButton::textColourId, juce::Colour(0xFFCCCCCC));
    sampleLoopBtn.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xFFCC0000));
    sampleLoopAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.getAPVTS(), "sampleLoop", sampleLoopBtn);
}

SchranzMachineEditor::~SchranzMachineEditor()
{
    setLookAndFeel(nullptr);
}

void SchranzMachineEditor::setupSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 16);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFF999999));
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}

void SchranzMachineEditor::setupLabel(juce::Label& label)
{
    label.setFont(juce::Font(13.0f).boldened());
    label.setColour(juce::Label::textColourId, juce::Colour(0xFFCC0000));
    label.setJustificationType(juce::Justification::centred);
}

void SchranzMachineEditor::updatePresetLabel()
{
    auto& pm = processorRef.getPresetManager();
    presetNameLabel.setText(pm.getPresetName(pm.getCurrentPresetIndex()), juce::dontSendNotification);
}

void SchranzMachineEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF111111));

    // Title
    g.setColour(juce::Colour(0xFFCC0000));
    g.setFont(juce::Font(22.0f).boldened());
    g.drawText("SCHRANZ MACHINE", getLocalBounds().removeFromTop(35), juce::Justification::centred);

    // Section dividers
    g.setColour(juce::Colour(0xFF2A2A2A));
    g.drawHorizontalLine(70, 10.0f, static_cast<float>(getWidth() - 10));
    g.drawHorizontalLine(240, 10.0f, static_cast<float>(getWidth() - 10));
    g.drawHorizontalLine(320, 10.0f, static_cast<float>(getWidth() - 10));
}

void SchranzMachineEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    // Title area
    area.removeFromTop(30);

    // Preset bar
    auto presetArea = area.removeFromTop(35);
    prevPresetBtn.setBounds(presetArea.removeFromLeft(40));
    nextPresetBtn.setBounds(presetArea.removeFromRight(40));
    presetNameLabel.setBounds(presetArea);

    area.removeFromTop(5);

    // Oscillators
    auto oscArea = area.removeFromTop(155);
    int oscWidth = oscArea.getWidth() / 2;

    // Osc 1
    auto osc1Area = oscArea.removeFromLeft(oscWidth);
    osc1Label.setBounds(osc1Area.removeFromTop(20));
    osc1TypeBox.setBounds(osc1Area.removeFromTop(25).reduced(20, 0));
    auto osc1Knobs = osc1Area;
    int knobW = osc1Knobs.getWidth() / 2;
    osc1GainSlider.setBounds(osc1Knobs.removeFromLeft(knobW));
    osc1DetuneSlider.setBounds(osc1Knobs);

    // Osc 2
    auto osc2Area = oscArea;
    osc2Label.setBounds(osc2Area.removeFromTop(20));
    osc2TypeBox.setBounds(osc2Area.removeFromTop(25).reduced(20, 0));
    auto osc2Knobs = osc2Area;
    knobW = osc2Knobs.getWidth() / 2;
    osc2GainSlider.setBounds(osc2Knobs.removeFromLeft(knobW));
    osc2DetuneSlider.setBounds(osc2Knobs);

    area.removeFromTop(5);

    // Sample drop area
    auto sampleArea = area.removeFromTop(65);
    dragDropArea.setBounds(sampleArea.removeFromLeft(sampleArea.getWidth() - 80));
    sampleLoopBtn.setBounds(sampleArea.reduced(5));

    area.removeFromTop(10);

    // Effects row
    auto fxArea = area.removeFromTop(180);
    int sectionW = fxArea.getWidth() / 4;

    // ADSR
    auto envArea = fxArea.removeFromLeft(sectionW);
    envLabel.setBounds(envArea.removeFromTop(20));
    int envKnobW = envArea.getWidth() / 2;
    int envKnobH = envArea.getHeight() / 2;
    attackSlider.setBounds(envArea.getX(), envArea.getY(), envKnobW, envKnobH);
    decaySlider.setBounds(envArea.getX() + envKnobW, envArea.getY(), envKnobW, envKnobH);
    sustainSlider.setBounds(envArea.getX(), envArea.getY() + envKnobH, envKnobW, envKnobH);
    releaseSlider.setBounds(envArea.getX() + envKnobW, envArea.getY() + envKnobH, envKnobW, envKnobH);

    // Distortion
    auto distArea = fxArea.removeFromLeft(sectionW);
    distLabel.setBounds(distArea.removeFromTop(20));
    distTypeBox.setBounds(distArea.removeFromTop(25).reduced(10, 0));
    int distKnobW = distArea.getWidth() / 2;
    driveSlider.setBounds(distArea.removeFromLeft(distKnobW));
    distMixSlider.setBounds(distArea);

    // Crusher
    auto crushArea = fxArea.removeFromLeft(sectionW);
    crushLabel.setBounds(crushArea.removeFromTop(20));
    int crushKnobW = crushArea.getWidth() / 2;
    crushBitsSlider.setBounds(crushArea.removeFromLeft(crushKnobW));
    crushRateSlider.setBounds(crushArea);

    // Filter
    auto filterArea = fxArea;
    filterLabel.setBounds(filterArea.removeFromTop(20));
    filterTypeBox.setBounds(filterArea.removeFromTop(25).reduced(10, 0));
    int filterKnobW = filterArea.getWidth() / 3;
    cutoffSlider.setBounds(filterArea.removeFromLeft(filterKnobW));
    resonanceSlider.setBounds(filterArea.removeFromLeft(filterKnobW));
    filterEnvSlider.setBounds(filterArea);

    area.removeFromTop(5);

    // Master
    auto masterArea = area.removeFromTop(40);
    masterLabel.setBounds(masterArea.removeFromLeft(70));
    masterSlider.setBounds(masterArea);
}
