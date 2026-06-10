#include "PluginEditor.h"

// ======================= LookAndFeel =======================
SchranzLookAndFeel::SchranzLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(kBgDark));
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(kAccent));
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(kBorder));
    setColour(juce::Slider::thumbColourId, juce::Colour(kAccent));
    setColour(juce::Label::textColourId, juce::Colour(kTextBright));
    setColour(juce::ComboBox::backgroundColourId, juce::Colour(kBgSection));
    setColour(juce::ComboBox::textColourId, juce::Colour(kTextBright));
    setColour(juce::ComboBox::outlineColourId, juce::Colour(kBorder));
    setColour(juce::ComboBox::arrowColourId, juce::Colour(kAccent));
    setColour(juce::TextButton::buttonColourId, juce::Colour(kBgSection));
    setColour(juce::TextButton::textColourOffId, juce::Colour(kTextBright));
    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(kBgPanel));
    setColour(juce::PopupMenu::textColourId, juce::Colour(kTextBright));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(kAccentDim));
    setColour(juce::ToggleButton::textColourId, juce::Colour(kTextDim));
    setColour(juce::ToggleButton::tickColourId, juce::Colour(kAccent));
}

void SchranzLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos, float startAngle, float endAngle,
                                           juce::Slider& /*slider*/)
{
    float radius = static_cast<float>(juce::jmin(width / 2, height / 2)) - 6.0f;
    float cx = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    float cy = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
    float angle = startAngle + sliderPos * (endAngle - startAngle);

    // Outer ring shadow
    g.setColour(juce::Colour(0xFF050505));
    g.fillEllipse(cx - radius - 2, cy - radius - 2, (radius + 2) * 2, (radius + 2) * 2);

    // Background circle
    g.setColour(juce::Colour(kBgSection));
    g.fillEllipse(cx - radius, cy - radius, radius * 2, radius * 2);

    // Background arc
    juce::Path bgArc;
    bgArc.addCentredArc(cx, cy, radius - 2, radius - 2, 0.0f, startAngle, endAngle, true);
    g.setColour(juce::Colour(kBorder));
    g.strokePath(bgArc, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved));

    // Value arc with glow
    if (sliderPos > 0.001f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc(cx, cy, radius - 2, radius - 2, 0.0f, startAngle, angle, true);
        g.setColour(juce::Colour(kAccent));
        g.strokePath(valueArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved));

        g.setColour(juce::Colour(kAccent).withAlpha(0.15f));
        g.strokePath(valueArc, juce::PathStrokeType(8.0f, juce::PathStrokeType::curved));
    }

    // Pointer line
    float lineLen = radius - 8.0f;
    juce::Point<float> tip(cx + lineLen * std::cos(angle - juce::MathConstants<float>::halfPi),
                            cy + lineLen * std::sin(angle - juce::MathConstants<float>::halfPi));
    juce::Point<float> base(cx + 4.0f * std::cos(angle - juce::MathConstants<float>::halfPi),
                             cy + 4.0f * std::sin(angle - juce::MathConstants<float>::halfPi));
    g.setColour(juce::Colours::white);
    g.drawLine(juce::Line<float>(base, tip), 2.0f);
}

void SchranzLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                           juce::Slider::SliderStyle style, juce::Slider& /*slider*/)
{
    if (style == juce::Slider::LinearHorizontal)
    {
        float trackY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        float trackH = 4.0f;

        g.setColour(juce::Colour(kBorder));
        g.fillRoundedRectangle(static_cast<float>(x), trackY - trackH * 0.5f,
                               static_cast<float>(width), trackH, 2.0f);

        float fillW = sliderPos - static_cast<float>(x);
        g.setColour(juce::Colour(kAccent));
        g.fillRoundedRectangle(static_cast<float>(x), trackY - trackH * 0.5f, fillW, trackH, 2.0f);

        g.setColour(juce::Colours::white);
        g.fillEllipse(sliderPos - 6.0f, trackY - 6.0f, 12.0f, 12.0f);
        g.setColour(juce::Colour(kAccent));
        g.fillEllipse(sliderPos - 4.0f, trackY - 4.0f, 8.0f, 8.0f);
    }
}

void SchranzLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                               const juce::Colour&,
                                               bool isHighlighted, bool isDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
    auto baseColour = isDown ? juce::Colour(kAccentDim) :
                      isHighlighted ? juce::Colour(kBgSection).brighter(0.1f) :
                      juce::Colour(kBgSection);
    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(juce::Colour(kBorder));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
}

void SchranzLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                                       int, int, int, int, juce::ComboBox& /*box*/)
{
    auto bounds = juce::Rectangle<float>(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    g.setColour(juce::Colour(kBgSection));
    g.fillRoundedRectangle(bounds, 3.0f);
    g.setColour(juce::Colour(kBorder));
    g.drawRoundedRectangle(bounds.reduced(0.5f), 3.0f, 1.0f);

    // Arrow
    float arrowX = static_cast<float>(width) - 16.0f;
    float arrowY = static_cast<float>(height) * 0.5f;
    juce::Path arrow;
    arrow.addTriangle(arrowX - 4, arrowY - 2, arrowX + 4, arrowY - 2, arrowX, arrowY + 3);
    g.setColour(juce::Colour(kAccent));
    g.fillPath(arrow);
}

// ======================= SectionPanel =======================
SectionPanel::SectionPanel(const juce::String& t) : title(t) {}

void SectionPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colour(SchranzLookAndFeel::kBgPanel));
    g.fillRoundedRectangle(bounds, 5.0f);
    g.setColour(juce::Colour(SchranzLookAndFeel::kBorder));
    g.drawRoundedRectangle(bounds.reduced(0.5f), 5.0f, 1.0f);

    g.setColour(juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.8f));
    g.setFont(juce::Font(10.0f).boldened());
    g.drawText(title, bounds.removeFromTop(18.0f).reduced(6.0f, 0.0f), juce::Justification::centredLeft);
}

juce::Rectangle<int> SectionPanel::getContentArea() const
{
    return getBounds().reduced(4).withTrimmedTop(18);
}

// ======================= DragDropArea =======================
DragDropArea::DragDropArea(SchranzMachineProcessor& p) : processor(p) {}

void DragDropArea::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(isDragOver ? juce::Colour(0xFF1A0505) : juce::Colour(SchranzLookAndFeel::kBgSection));
    g.fillRoundedRectangle(bounds, 4.0f);

    if (isDragOver)
    {
        g.setColour(juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.5f));
        g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 2.0f);
    }
    else
    {
        g.setColour(juce::Colour(SchranzLookAndFeel::kBorder));
        float dashLengths[] = { 6.0f, 4.0f };
        g.drawDashedLine(juce::Line<float>(bounds.getX() + 8, bounds.getCentreY(), bounds.getRight() - 8, bounds.getCentreY()),
                         dashLengths, 2, 0.5f);
    }

    auto sampleName = processor.getLoadedSampleName();
    g.setFont(12.0f);
    if (sampleName.isEmpty())
    {
        g.setColour(juce::Colour(SchranzLookAndFeel::kTextDim));
        g.drawText("Drop sample here", bounds, juce::Justification::centred);
    }
    else
    {
        g.setColour(juce::Colour(SchranzLookAndFeel::kTextBright));
        g.drawText(sampleName, bounds, juce::Justification::centred);
    }
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

void DragDropArea::fileDragEnter(const juce::StringArray&, int, int) { isDragOver = true; repaint(); }
void DragDropArea::fileDragExit(const juce::StringArray&) { isDragOver = false; repaint(); }

// ======================= Editor =======================
SchranzMachineEditor::SchranzMachineEditor(SchranzMachineProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      keyboard(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard),
      dragDropArea(p)
{
    setLookAndFeel(&schranzLnf);
    setResizable(true, true);
    setResizeLimits(960, 700, 1800, 1300);
    setSize(1080, 800);
    setWantsKeyboardFocus(true);
    addKeyListener(this);

    // Keyboard setup — 4 visible octaves, Y/X shift the range up/down
    keyboard.setAvailableRange(kbRangeLow, kbRangeLow + 48);
    keyboard.setKeyPressBaseOctave(kbBaseOctave);
    keyboard.setLowestVisibleKey(kbRangeLow);
    keyboard.onOctaveShift = [this](int dir) { shiftKeyboardOctave(dir); };
    keyboard.setOctaveForMiddleC(4);
    keyboard.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colour(0xFF1A1A1A));
    keyboard.setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour(0xFF050505));
    keyboard.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colour(0xFF2A2A2A));
    keyboard.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.6f));
    keyboard.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.2f));
    keyboard.setColour(juce::MidiKeyboardComponent::upDownButtonBackgroundColourId, juce::Colour(0xFF111111));
    keyboard.setColour(juce::MidiKeyboardComponent::upDownButtonArrowColourId, juce::Colour(SchranzLookAndFeel::kAccent));
    addAndMakeVisible(keyboard);

    // Octave shift controls (buttons + Y/X shortcut)
    addAndMakeVisible(octaveDownBtn);
    addAndMakeVisible(octaveUpBtn);
    addAndMakeVisible(octaveLabel);
    octaveLabel.setJustificationType(juce::Justification::centred);
    octaveLabel.setFont(juce::Font(10.0f).boldened());
    octaveLabel.setColour(juce::Label::textColourId, juce::Colour(SchranzLookAndFeel::kTextDim));
    octaveDownBtn.setTooltip("Shift keyboard one octave down (shortcut: Y)");
    octaveUpBtn.setTooltip("Shift keyboard one octave up (shortcut: X)");
    octaveDownBtn.onClick = [this] { shiftKeyboardOctave(-1); };
    octaveUpBtn.onClick   = [this] { shiftKeyboardOctave(+1); };
    {
        int displayOctave = kbRangeLow / 12 - 1;
        octaveLabel.setText("OCT " + juce::String(displayOctave) + "-" + juce::String(displayOctave + 4),
                            juce::dontSendNotification);
    }

    // Preset bar
    addAndMakeVisible(prevPresetBtn);
    addAndMakeVisible(nextPresetBtn);
    addAndMakeVisible(presetNameLabel);
    presetNameLabel.setJustificationType(juce::Justification::centred);
    presetNameLabel.setFont(juce::Font(14.0f).boldened());
    presetNameLabel.setColour(juce::Label::textColourId, juce::Colour(SchranzLookAndFeel::kAccent));
    updatePresetLabel();
    prevPresetBtn.onClick = [this] {
        processorRef.getPresetManager().previousPreset();
        updatePresetLabel();
        refreshPresetCategoryList();
        refreshPresetNameList();
    };
    nextPresetBtn.onClick = [this] {
        processorRef.getPresetManager().nextPreset();
        updatePresetLabel();
        refreshPresetCategoryList();
        refreshPresetNameList();
    };

    // Preset browser
    addAndMakeVisible(presetCategoryLabel); setupSectionLabel(presetCategoryLabel);
    addAndMakeVisible(presetBrowserLabel); setupSectionLabel(presetBrowserLabel);
    addAndMakeVisible(presetCategoryBox);
    addAndMakeVisible(presetNameBox);
    refreshPresetCategoryList();
    refreshPresetNameList();
    presetCategoryBox.onChange = [this] { refreshPresetNameList(); };
    presetNameBox.onChange = [this] {
        int presetIdx = presetNameBox.getSelectedId() - 1;
        if (presetIdx >= 0 && presetIdx < processorRef.getPresetManager().getNumPresets())
        {
            processorRef.getPresetManager().loadPreset(presetIdx);
            updatePresetLabel();
            refreshSuggestions();
        }
    };

    // MIDI Pattern rack
    addAndMakeVisible(patternRackSection);
    addAndMakeVisible(patternCategoryLabel); setupSectionLabel(patternCategoryLabel);
    addAndMakeVisible(patternNameLabelUI); setupSectionLabel(patternNameLabelUI);
    addAndMakeVisible(patternBpmLabel); setupSectionLabel(patternBpmLabel);
    addAndMakeVisible(patternCategoryBox);
    addAndMakeVisible(patternNameBox);
    addAndMakeVisible(patternPlayBtn);
    addAndMakeVisible(patternBpmSlider);
    patternBpmSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    patternBpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 14);
    patternBpmSlider.setRange(60.0, 200.0, 1.0);
    patternBpmSlider.setValue(140.0);
    patternBpmSlider.onValueChange = [this] {
        processorRef.setPatternBpm(static_cast<float>(patternBpmSlider.getValue()));
    };
    refreshPatternCategoryList();
    refreshPatternNameList();
    patternCategoryBox.onChange = [this] { refreshPatternNameList(); refreshSuggestions(); };
    patternPlayBtn.onClick = [this] {
        if (processorRef.isPatternPlaying())
        {
            processorRef.stopPatternPlayback();
            patternPlayBtn.setButtonText("PLAY");
        }
        else
        {
            int patIdx = patternNameBox.getSelectedId() - 1;
            if (patIdx >= 0 && patIdx < processorRef.getPatternBank().getNumPatterns())
            {
                processorRef.startPatternPlayback(patIdx);
                patternPlayBtn.setButtonText("STOP");
            }
        }
    };

    // AI Suggestions
    suggestionEngine = std::make_unique<SuggestionEngine>(processorRef.getPresetManager(),
                                                          processorRef.getPatternBank());
    addAndMakeVisible(suggestSection);
    for (int i = 0; i < 8; ++i)
    {
        auto* btn = new juce::TextButton("");
        btn->setColour(juce::TextButton::buttonColourId, juce::Colour(SchranzLookAndFeel::kBgSection));
        btn->setColour(juce::TextButton::textColourOffId, juce::Colour(SchranzLookAndFeel::kTextBright));
        addAndMakeVisible(btn);
        suggestionBtns.add(btn);
        btn->onClick = [this, i] {
            if (i >= suggestionBtns.size()) return;
            auto text = suggestionBtns[i]->getButtonText();
            if (text.isEmpty()) return;
            auto suggestions = suggestionEngine->getSuggestions(
                processorRef.getPresetManager().getCurrentPresetIndex(),
                patternNameBox.getSelectedId() - 1);
            if (i < static_cast<int>(suggestions.size()))
            {
                auto& s = suggestions[static_cast<size_t>(i)];
                if (s.isPattern && s.patternIndex >= 0)
                {
                    processorRef.startPatternPlayback(s.patternIndex);
                    patternPlayBtn.setButtonText("STOP");
                    patternNameBox.setSelectedId(s.patternIndex + 1, juce::sendNotification);
                }
                else if (s.presetIndex >= 0)
                {
                    processorRef.getPresetManager().loadPreset(s.presetIndex);
                    updatePresetLabel();
                    refreshPresetNameList();
                }
                refreshSuggestions();
            }
        };
    }
    refreshSuggestions();

    // Section panels
    for (auto* panel : { &oscSection, &sampleSection, &envSection, &distSection, &crushSection,
                          &filterSection, &delaySection, &reverbSection, &compSection, &chorusSection,
                          &phaserSection, &eqSection, &ringModSection, &wsSection })
        addAndMakeVisible(panel);

    auto& apvts = processorRef.getAPVTS();

    // Helper lambdas
    auto addKnob = [&](juce::Slider& s, juce::Label& l, const juce::String& paramId)
    {
        addAndMakeVisible(s);
        setupKnob(s);
        addAndMakeVisible(l);
        setupKnobLabel(l);
    };

    auto attachSlider = [&](auto& attach, const juce::String& id, juce::Slider& s) {
        attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, id, s);
    };

    auto addCombo = [&](juce::ComboBox& box, const juce::StringArray& items, auto& attach, const juce::String& paramId) {
        addAndMakeVisible(box);
        box.addItemList(items, 1);
        attach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, paramId, box);
    };

    // Osc 1
    addAndMakeVisible(osc1TitleLabel); setupSectionLabel(osc1TitleLabel);
    addCombo(osc1TypeBox, {"Sine", "Saw", "Square", "Noise", "Sample"}, osc1TypeAttach, "osc1Type");
    addKnob(osc1GainSlider, osc1GainLabel, "osc1Gain");
    attachSlider(osc1GainAttach, "osc1Gain", osc1GainSlider);
    addKnob(osc1DetuneSlider, osc1DetuneLabel, "osc1Detune");
    attachSlider(osc1DetuneAttach, "osc1Detune", osc1DetuneSlider);

    // Osc 2
    addAndMakeVisible(osc2TitleLabel); setupSectionLabel(osc2TitleLabel);
    addCombo(osc2TypeBox, {"Sine", "Saw", "Square", "Noise", "Sample"}, osc2TypeAttach, "osc2Type");
    addKnob(osc2GainSlider, osc2GainLabel, "osc2Gain");
    attachSlider(osc2GainAttach, "osc2Gain", osc2GainSlider);
    addKnob(osc2DetuneSlider, osc2DetuneLabel, "osc2Detune");
    attachSlider(osc2DetuneAttach, "osc2Detune", osc2DetuneSlider);

    // Envelope
    addKnob(attackSlider, attackLabel, "ampAttack");   attachSlider(attackAttach, "ampAttack", attackSlider);
    addKnob(decaySlider, decayLabel, "ampDecay");      attachSlider(decayAttach, "ampDecay", decaySlider);
    addKnob(sustainSlider, sustainLabel, "ampSustain"); attachSlider(sustainAttach, "ampSustain", sustainSlider);
    addKnob(releaseSlider, releaseLabel, "ampRelease"); attachSlider(releaseAttach, "ampRelease", releaseSlider);

    // Distortion
    addCombo(distTypeBox, {"Hard Clip", "Soft Clip", "Foldback", "Bit Crush", "Rectify", "Scream"}, distTypeAttach, "distType");
    addKnob(driveSlider, driveLabel, "distDrive");     attachSlider(driveAttach, "distDrive", driveSlider);
    addKnob(distMixSlider, distMixLabel, "distMix");   attachSlider(distMixAttach, "distMix", distMixSlider);

    // Crusher
    addKnob(crushBitsSlider, crushBitsLabel, "crushBits"); attachSlider(crushBitsAttach, "crushBits", crushBitsSlider);
    addKnob(crushRateSlider, crushRateLabel, "crushRate"); attachSlider(crushRateAttach, "crushRate", crushRateSlider);

    // Filter
    addCombo(filterTypeBox, {"Low Pass", "High Pass", "Band Pass", "Notch"}, filterTypeAttach, "filterType");
    addKnob(cutoffSlider, cutoffLabel, "filterCutoff");     attachSlider(cutoffAttach, "filterCutoff", cutoffSlider);
    addKnob(resonanceSlider, resLabel, "filterResonance");  attachSlider(resonanceAttach, "filterResonance", resonanceSlider);
    addKnob(filterEnvSlider, fEnvLabel, "filterEnvAmount"); attachSlider(filterEnvAttach, "filterEnvAmount", filterEnvSlider);

    // Delay
    addKnob(delayTimeSlider, delayTimeLabel, "delayTime"); attachSlider(delayTimeAttach, "delayTime", delayTimeSlider);
    addKnob(delayFbSlider, delayFbLabel, "delayFeedback"); attachSlider(delayFbAttach, "delayFeedback", delayFbSlider);
    addKnob(delayMixSlider, delayMixLabel, "delayMix");    attachSlider(delayMixAttach, "delayMix", delayMixSlider);
    addAndMakeVisible(delayPPBtn);
    delayPPBtn.setColour(juce::ToggleButton::textColourId, juce::Colour(SchranzLookAndFeel::kTextDim));
    delayPPBtn.setColour(juce::ToggleButton::tickColourId, juce::Colour(SchranzLookAndFeel::kAccent));
    delayPPAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "delayPingPong", delayPPBtn);

    // Reverb
    addKnob(reverbSizeSlider, rvSizeLabel, "reverbSize");    attachSlider(rvSizeAttach, "reverbSize", reverbSizeSlider);
    addKnob(reverbDampSlider, rvDampLabel, "reverbDamping");  attachSlider(rvDampAttach, "reverbDamping", reverbDampSlider);
    addKnob(reverbMixSlider, rvMixLabel, "reverbMix");        attachSlider(rvMixAttach, "reverbMix", reverbMixSlider);
    addKnob(reverbWidthSlider, rvWidthLabel, "reverbWidth");  attachSlider(rvWidthAttach, "reverbWidth", reverbWidthSlider);

    // Compressor
    addKnob(compThreshSlider, compThLabel, "compThreshold");   attachSlider(compThreshAttach, "compThreshold", compThreshSlider);
    addKnob(compRatioSlider, compRaLabel, "compRatio");        attachSlider(compRatioAttach, "compRatio", compRatioSlider);
    addKnob(compAttackSlider, compALabel, "compAttack");       attachSlider(compAttackAttach, "compAttack", compAttackSlider);
    addKnob(compReleaseSlider, compRLabel, "compRelease");     attachSlider(compReleaseAttach, "compRelease", compReleaseSlider);
    addKnob(compMakeupSlider, compMLabel, "compMakeup");       attachSlider(compMakeupAttach, "compMakeup", compMakeupSlider);

    // Chorus
    addKnob(chorusRateSlider, chRateLabel, "chorusRate");   attachSlider(chRateAttach, "chorusRate", chorusRateSlider);
    addKnob(chorusDepthSlider, chDepthLabel, "chorusDepth"); attachSlider(chDepthAttach, "chorusDepth", chorusDepthSlider);
    addKnob(chorusMixSlider, chMixLabel, "chorusMix");       attachSlider(chMixAttach, "chorusMix", chorusMixSlider);

    // Phaser
    addKnob(phaserRateSlider, phRateLabel, "phaserRate");   attachSlider(phRateAttach, "phaserRate", phaserRateSlider);
    addKnob(phaserDepthSlider, phDepthLabel, "phaserDepth"); attachSlider(phDepthAttach, "phaserDepth", phaserDepthSlider);
    addKnob(phaserMixSlider, phMixLabel, "phaserMix");       attachSlider(phMixAttach, "phaserMix", phaserMixSlider);
    addKnob(phaserFbSlider, phFbLabel, "phaserFeedback");    attachSlider(phFbAttach, "phaserFeedback", phaserFbSlider);
    phaserRateSlider.setTooltip("Phaser LFO speed (only audible when MIX > 0)");
    phaserDepthSlider.setTooltip("Sweep range (only audible when MIX > 0)");
    phaserFbSlider.setTooltip("Resonance (only audible when MIX > 0)");

    // EQ
    addKnob(eqLowSlider, eqLowLabel, "eqLowGain");     attachSlider(eqLowAttach, "eqLowGain", eqLowSlider);
    addKnob(eqMidSlider, eqMidLabel, "eqMidGain");     attachSlider(eqMidAttach, "eqMidGain", eqMidSlider);
    addKnob(eqHighSlider, eqHighLabel, "eqHighGain");   attachSlider(eqHighAttach, "eqHighGain", eqHighSlider);
    addKnob(eqMidFreqSlider, eqFreqLabel, "eqMidFreq"); attachSlider(eqMidFreqAttach, "eqMidFreq", eqMidFreqSlider);

    // Ring Mod
    addKnob(ringFreqSlider, ringFreqLabel, "ringModFreq"); attachSlider(ringFreqAttach, "ringModFreq", ringFreqSlider);
    addKnob(ringMixSlider, ringMixLabel, "ringModMix");    attachSlider(ringMixAttach, "ringModMix", ringMixSlider);
    ringFreqSlider.setTooltip("Carrier frequency (only audible when MIX > 0)");
    ringMixSlider.setTooltip("Turn UP to hear metallic ring mod effect");

    // Waveshaper
    addCombo(wsTypeBox, {"Tanh", "Sin", "Abs", "Cubic"}, wsTypeAttach, "wsType");
    addKnob(wsAmountSlider, wsAmountLabel, "wsAmount"); attachSlider(wsAmountAttach, "wsAmount", wsAmountSlider);
    wsAmountSlider.setTooltip("Turn UP to apply waveshaping (type changes character)");
    wsTypeBox.setTooltip("Shape character (only audible when AMOUNT > 0)");

    // EQ tooltips
    eqMidFreqSlider.setTooltip("Mid band center frequency (only audible when MID gain != 0)");

    // Master
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    masterSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    masterAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "masterGain", masterSlider);

    // Sample
    addAndMakeVisible(dragDropArea);
    addAndMakeVisible(sampleLoopBtn);
    sampleLoopBtn.setColour(juce::ToggleButton::textColourId, juce::Colour(SchranzLookAndFeel::kTextDim));
    sampleLoopBtn.setColour(juce::ToggleButton::tickColourId, juce::Colour(SchranzLookAndFeel::kAccent));
    sampleLoopAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "sampleLoop", sampleLoopBtn);

    startTimerHz(30);
}

SchranzMachineEditor::~SchranzMachineEditor()
{
    stopTimer();
    removeKeyListener(this);
    setLookAndFeel(nullptr);
}

void SchranzMachineEditor::timerCallback()
{
    // Only claim focus for computer-key playing when the mouse is over the
    // plugin and no editable widget (combo box popup, text entry) is active.
    if (isMouseOverOrDragging(true))
    {
        auto* focused = juce::Component::getCurrentlyFocusedComponent();
        if (focused == nullptr || focused == this)
            keyboard.grabKeyboardFocus();
    }
}

bool SchranzMachineEditor::keyPressed(const juce::KeyPress& key, juce::Component*)
{
    auto code = key.getKeyCode();
    if (code == 'Y' || code == 'y') { shiftKeyboardOctave(-1); return true; }
    if (code == 'X' || code == 'x') { shiftKeyboardOctave(+1); return true; }
    return false;
}

void SchranzMachineEditor::shiftKeyboardOctave(int direction)
{
    // Move the visible/playable range by one octave, clamped to MIDI 0..120
    int newLow = juce::jlimit(0, 72, kbRangeLow + direction * 12);
    if (newLow == kbRangeLow) return;
    kbRangeLow = newLow;
    kbBaseOctave = juce::jlimit(0, 8, kbBaseOctave + direction);

    keyboard.setAvailableRange(kbRangeLow, juce::jmin(120, kbRangeLow + 48));
    keyboard.setLowestVisibleKey(kbRangeLow);
    keyboard.setKeyPressBaseOctave(kbBaseOctave);

    int displayOctave = kbRangeLow / 12 - 1;  // C1 == MIDI 24 -> octave 1
    octaveLabel.setText("OCT " + juce::String(displayOctave) + "-" + juce::String(displayOctave + 4),
                        juce::dontSendNotification);
    keyboard.grabKeyboardFocus();
}

void SchranzMachineEditor::setupKnob(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, true, this);
}

void SchranzMachineEditor::setupKnobLabel(juce::Label& label)
{
    label.setFont(juce::Font(9.0f));
    label.setColour(juce::Label::textColourId, juce::Colour(SchranzLookAndFeel::kTextDim));
    label.setJustificationType(juce::Justification::centred);
}

void SchranzMachineEditor::setupSectionLabel(juce::Label& label)
{
    label.setFont(juce::Font(10.0f).boldened());
    label.setColour(juce::Label::textColourId, juce::Colour(SchranzLookAndFeel::kTextBright));
    label.setJustificationType(juce::Justification::centred);
}

void SchranzMachineEditor::updatePresetLabel()
{
    auto& pm = processorRef.getPresetManager();
    int idx = pm.getCurrentPresetIndex();
    juce::String cat = pm.getPresetCategory(idx);
    juce::String text = pm.getPresetName(idx);
    if (cat.isNotEmpty()) text += "  [" + cat + "]";
    presetNameLabel.setText(text, juce::dontSendNotification);
}

void SchranzMachineEditor::refreshPresetCategoryList()
{
    auto& pm = processorRef.getPresetManager();
    auto cats = pm.getCategoryNames();
    juce::String currentSel = presetCategoryBox.getText();
    presetCategoryBox.clear(juce::dontSendNotification);
    for (int i = 0; i < cats.size(); ++i)
        presetCategoryBox.addItem(cats[i], i + 1);
    if (presetCategoryBox.getNumItems() > 0)
    {
        juce::String currentCat = pm.getPresetCategory(pm.getCurrentPresetIndex());
        int found = cats.indexOf(currentCat);
        if (found < 0) found = 0;
        presetCategoryBox.setSelectedId(found + 1, juce::dontSendNotification);
    }
}

void SchranzMachineEditor::refreshPresetNameList()
{
    auto& pm = processorRef.getPresetManager();
    juce::String category = presetCategoryBox.getText();
    if (category.isEmpty()) category = "All";
    auto indices = pm.getPresetIndicesInCategory(category);
    presetNameBox.clear(juce::dontSendNotification);
    for (auto idx : indices)
        presetNameBox.addItem(pm.getPresetName(idx), idx + 1);
    int currentIdx = pm.getCurrentPresetIndex();
    if (indices.contains(currentIdx))
        presetNameBox.setSelectedId(currentIdx + 1, juce::dontSendNotification);
    else if (presetNameBox.getNumItems() > 0)
        presetNameBox.setSelectedItemIndex(0, juce::dontSendNotification);
}

void SchranzMachineEditor::refreshPatternCategoryList()
{
    auto& pb = processorRef.getPatternBank();
    auto cats = pb.getCategoryNames();
    patternCategoryBox.clear(juce::dontSendNotification);
    juce::StringArray allCats;
    allCats.add("All");
    for (int i = 0; i < cats.size(); ++i)
        allCats.add(cats[i]);
    for (int i = 0; i < allCats.size(); ++i)
        patternCategoryBox.addItem(allCats[i], i + 1);
    if (patternCategoryBox.getNumItems() > 0)
        patternCategoryBox.setSelectedItemIndex(0, juce::dontSendNotification);
}

void SchranzMachineEditor::refreshSuggestions()
{
    if (!suggestionEngine) return;
    auto suggestions = suggestionEngine->getSuggestions(
        processorRef.getPresetManager().getCurrentPresetIndex(),
        patternNameBox.getSelectedId() - 1);

    for (int i = 0; i < suggestionBtns.size(); ++i)
    {
        if (i < static_cast<int>(suggestions.size()))
        {
            suggestionBtns[i]->setButtonText(suggestions[static_cast<size_t>(i)].label);
            suggestionBtns[i]->setVisible(true);
        }
        else
        {
            suggestionBtns[i]->setButtonText("");
            suggestionBtns[i]->setVisible(false);
        }
    }
}

void SchranzMachineEditor::refreshPatternNameList()
{
    auto& pb = processorRef.getPatternBank();
    juce::String category = patternCategoryBox.getText();
    patternNameBox.clear(juce::dontSendNotification);
    if (category == "All" || category.isEmpty())
    {
        for (int i = 0; i < pb.getNumPatterns(); ++i)
            patternNameBox.addItem(pb.getPattern(i).name, i + 1);
    }
    else
    {
        auto indices = pb.getPatternsInCategory(category);
        for (auto idx : indices)
            patternNameBox.addItem(pb.getPattern(idx).name, idx + 1);
    }
    if (patternNameBox.getNumItems() > 0)
        patternNameBox.setSelectedItemIndex(0, juce::dontSendNotification);
}

void SchranzMachineEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Dark gradient background with subtle radial highlight
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xFF101010), w * 0.5f, h * 0.3f,
        juce::Colour(0xFF060606), 0.0f, h, true));
    g.fillAll();

    // Subtle grid pattern for depth
    g.setColour(juce::Colour(0x05FFFFFF));
    for (float y = 0; y < h; y += 20.0f)
        g.drawHorizontalLine(static_cast<int>(y), 0.0f, w);

    // Title bar with gradient
    auto titleArea = bounds.removeFromTop(42.0f);
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xFF151515), 0.0f, titleArea.getY(),
        juce::Colour(0xFF0C0C0C), 0.0f, titleArea.getBottom(), false));
    g.fillRect(titleArea);

    // Title glow
    g.setColour(juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.08f));
    g.fillRect(titleArea);

    g.setColour(juce::Colour(SchranzLookAndFeel::kAccent));
    g.setFont(juce::Font(22.0f).boldened());
    g.drawText("SCHRANZ MACHINE", titleArea.reduced(12, 0), juce::Justification::centredLeft);

    g.setColour(juce::Colour(SchranzLookAndFeel::kTextDim));
    g.setFont(juce::Font(10.0f));
    g.drawText("v2.0  |  552 Presets  |  114 MIDI Patterns", titleArea.reduced(12, 0), juce::Justification::centredRight);

    // Accent line under title with glow
    g.setColour(juce::Colour(SchranzLookAndFeel::kAccent));
    g.fillRect(0.0f, 42.0f, w, 2.0f);
    g.setColour(juce::Colour(SchranzLookAndFeel::kAccent).withAlpha(0.12f));
    g.fillRect(0.0f, 42.0f, w, 6.0f);

    // Master label
    g.setColour(juce::Colour(SchranzLookAndFeel::kTextDim));
    g.setFont(juce::Font(10.0f).boldened());
    g.drawText("MASTER", masterSlider.getBounds().withWidth(60).translated(-64, 0).toFloat(), juce::Justification::centredRight);
}

void SchranzMachineEditor::resized()
{
    auto area = getLocalBounds();

    // Title bar
    auto titleBar = area.removeFromTop(42);
    (void)titleBar;

    // Preset bar (browser: prev | category | name | next | label)
    auto presetBar = area.removeFromTop(28).reduced(8, 2);
    prevPresetBtn.setBounds(presetBar.removeFromLeft(26));
    presetBar.removeFromLeft(4);
    presetCategoryLabel.setBounds(presetBar.removeFromLeft(60));
    presetCategoryBox.setBounds(presetBar.removeFromLeft(120));
    presetBar.removeFromLeft(6);
    presetBrowserLabel.setBounds(presetBar.removeFromLeft(50));
    presetCategoryBox.setBounds(presetCategoryBox.getBounds());
    nextPresetBtn.setBounds(presetBar.removeFromRight(26));
    presetBar.removeFromRight(4);
    presetNameLabel.setBounds(presetBar.removeFromRight(220));
    presetBar.removeFromRight(6);
    presetNameBox.setBounds(presetBar);

    // Keyboard at bottom (4 visible octaves, 64px tall)
    auto keyboardArea = area.removeFromBottom(64);
    keyboard.setBounds(keyboardArea);

    // Octave control strip just above keyboard
    auto octaveStrip = area.removeFromBottom(24).reduced(8, 2);
    octaveDownBtn.setBounds(octaveStrip.removeFromLeft(60));
    octaveStrip.removeFromLeft(4);
    octaveLabel.setBounds(octaveStrip.removeFromLeft(90));
    octaveStrip.removeFromLeft(4);
    octaveUpBtn.setBounds(octaveStrip.removeFromLeft(60));

    // MIDI Pattern rack above keyboard (60 px tall)
    auto patternRack = area.removeFromBottom(64).reduced(6, 2);
    patternRackSection.setBounds(patternRack);
    {
        auto content = patternRackSection.getContentArea().reduced(4, 2);
        patternCategoryLabel.setBounds(content.removeFromLeft(50));
        patternCategoryBox.setBounds(content.removeFromLeft(110));
        content.removeFromLeft(6);
        patternNameLabelUI.setBounds(content.removeFromLeft(60));
        patternNameBox.setBounds(content.removeFromLeft(220));
        content.removeFromLeft(8);
        patternPlayBtn.setBounds(content.removeFromLeft(70).reduced(0, 2));
        content.removeFromLeft(10);
        patternBpmLabel.setBounds(content.removeFromLeft(34));
        patternBpmSlider.setBounds(content.removeFromLeft(80));
    }

    // AI Suggestions above MIDI rack
    auto suggestArea = area.removeFromBottom(56).reduced(6, 2);
    suggestSection.setBounds(suggestArea);
    {
        auto content = suggestSection.getContentArea().reduced(4, 1);
        int btnW = content.getWidth() / 4;
        int btnH = content.getHeight() / 2;
        for (int i = 0; i < suggestionBtns.size(); ++i)
        {
            int col = i % 4;
            int row = i / 4;
            suggestionBtns[i]->setBounds(content.getX() + col * btnW + 2,
                                          content.getY() + row * btnH + 1,
                                          btnW - 4, btnH - 2);
        }
    }

    // Master slider
    auto masterBar = area.removeFromBottom(28).reduced(70, 2);
    masterSlider.setBounds(masterBar);

    auto main = area.reduced(6, 3);

    int rowHeight = juce::jmax(100, main.getHeight() / 4);

    // Row 1: Oscillators + Sample + Envelope
    auto row1 = main.removeFromTop(rowHeight);
    int row1W = row1.getWidth();
    oscSection.setBounds(row1.removeFromLeft(row1W * 45 / 100).reduced(2));
    sampleSection.setBounds(row1.removeFromLeft(row1W * 25 / 100).reduced(2));
    envSection.setBounds(row1.reduced(2));

    // Layout osc section content
    {
        auto content = oscSection.getContentArea();
        int halfW = content.getWidth() / 2;

        auto osc1 = content.removeFromLeft(halfW).reduced(2);
        osc1TitleLabel.setBounds(osc1.removeFromTop(14));
        osc1TypeBox.setBounds(osc1.removeFromTop(22).reduced(4, 0));
        int kw = osc1.getWidth() / 2;
        osc1GainSlider.setBounds(osc1.removeFromLeft(kw).withTrimmedBottom(12));
        osc1DetuneSlider.setBounds(osc1.withTrimmedBottom(12));
        osc1GainLabel.setBounds(osc1GainSlider.getX(), osc1GainSlider.getBottom() - 2, kw, 12);
        osc1DetuneLabel.setBounds(osc1DetuneSlider.getX(), osc1DetuneSlider.getBottom() - 2, osc1.getWidth(), 12);

        auto osc2 = content.reduced(2);
        osc2TitleLabel.setBounds(osc2.removeFromTop(14));
        osc2TypeBox.setBounds(osc2.removeFromTop(22).reduced(4, 0));
        kw = osc2.getWidth() / 2;
        osc2GainSlider.setBounds(osc2.removeFromLeft(kw).withTrimmedBottom(12));
        osc2DetuneSlider.setBounds(osc2.withTrimmedBottom(12));
        osc2GainLabel.setBounds(osc2GainSlider.getX(), osc2GainSlider.getBottom() - 2, kw, 12);
        osc2DetuneLabel.setBounds(osc2DetuneSlider.getX(), osc2DetuneSlider.getBottom() - 2, osc2.getWidth(), 12);
    }

    // Sample section
    {
        auto content = sampleSection.getContentArea().reduced(4);
        dragDropArea.setBounds(content.removeFromTop(content.getHeight() - 24));
        sampleLoopBtn.setBounds(content);
    }

    // Envelope section
    {
        auto content = envSection.getContentArea();
        int kw = content.getWidth() / 4;
        int kh = content.getHeight();
        auto placeKnob = [&](juce::Slider& s, juce::Label& l, int idx) {
            auto area2 = juce::Rectangle<int>(content.getX() + idx * kw, content.getY(), kw, kh);
            s.setBounds(area2.withTrimmedBottom(12));
            l.setBounds(area2.getX(), area2.getBottom() - 12, kw, 12);
        };
        placeKnob(attackSlider, attackLabel, 0);
        placeKnob(decaySlider, decayLabel, 1);
        placeKnob(sustainSlider, sustainLabel, 2);
        placeKnob(releaseSlider, releaseLabel, 3);
    }

    main.removeFromTop(4);

    // Row 2: Distortion + Crusher + Filter
    auto row2 = main.removeFromTop(rowHeight);
    int row2W = row2.getWidth();
    distSection.setBounds(row2.removeFromLeft(row2W / 3).reduced(2));
    crushSection.setBounds(row2.removeFromLeft(row2W / 3).reduced(2));
    filterSection.setBounds(row2.reduced(2));

    // Distortion layout
    {
        auto content = distSection.getContentArea().reduced(2);
        distTypeBox.setBounds(content.removeFromTop(22).reduced(4, 0));
        content.removeFromTop(2);
        int kw = content.getWidth() / 2;
        driveSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        distMixSlider.setBounds(content.withTrimmedBottom(12));
        driveLabel.setBounds(driveSlider.getX(), driveSlider.getBottom() - 2, kw, 12);
        distMixLabel.setBounds(distMixSlider.getX(), distMixSlider.getBottom() - 2, content.getWidth(), 12);
    }

    // Crusher layout
    {
        auto content = crushSection.getContentArea().reduced(2);
        content.removeFromTop(24);
        int kw = content.getWidth() / 2;
        crushBitsSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        crushRateSlider.setBounds(content.withTrimmedBottom(12));
        crushBitsLabel.setBounds(crushBitsSlider.getX(), crushBitsSlider.getBottom() - 2, kw, 12);
        crushRateLabel.setBounds(crushRateSlider.getX(), crushRateSlider.getBottom() - 2, content.getWidth(), 12);
    }

    // Filter layout
    {
        auto content = filterSection.getContentArea().reduced(2);
        filterTypeBox.setBounds(content.removeFromTop(22).reduced(4, 0));
        content.removeFromTop(2);
        int kw = content.getWidth() / 3;
        cutoffSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        resonanceSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        filterEnvSlider.setBounds(content.withTrimmedBottom(12));
        cutoffLabel.setBounds(cutoffSlider.getX(), cutoffSlider.getBottom() - 2, kw, 12);
        resLabel.setBounds(resonanceSlider.getX(), resonanceSlider.getBottom() - 2, kw, 12);
        fEnvLabel.setBounds(filterEnvSlider.getX(), filterEnvSlider.getBottom() - 2, content.getWidth(), 12);
    }

    main.removeFromTop(4);

    // Row 3: Delay + Reverb + Compressor + Chorus
    auto row3 = main.removeFromTop(rowHeight);
    int row3W = row3.getWidth();
    delaySection.setBounds(row3.removeFromLeft(row3W / 4).reduced(2));
    reverbSection.setBounds(row3.removeFromLeft(row3W / 4).reduced(2));
    compSection.setBounds(row3.removeFromLeft(row3W / 4).reduced(2));
    chorusSection.setBounds(row3.reduced(2));

    // Delay layout
    {
        auto content = delaySection.getContentArea().reduced(2);
        int kw = content.getWidth() / 3;
        auto knobArea = content.withTrimmedBottom(22);
        delayTimeSlider.setBounds(knobArea.removeFromLeft(kw).withTrimmedBottom(12));
        delayFbSlider.setBounds(knobArea.removeFromLeft(kw).withTrimmedBottom(12));
        delayMixSlider.setBounds(knobArea.withTrimmedBottom(12));
        delayTimeLabel.setBounds(delayTimeSlider.getX(), delayTimeSlider.getBottom() - 2, kw, 12);
        delayFbLabel.setBounds(delayFbSlider.getX(), delayFbSlider.getBottom() - 2, kw, 12);
        delayMixLabel.setBounds(delayMixSlider.getX(), delayMixSlider.getBottom() - 2, knobArea.getWidth(), 12);
        delayPPBtn.setBounds(content.removeFromBottom(20).reduced(4, 0));
    }

    // Reverb layout
    {
        auto content = reverbSection.getContentArea().reduced(2);
        int kw = content.getWidth() / 2;
        int kh = content.getHeight() / 2;
        auto placeKnob = [&](juce::Slider& s, juce::Label& l, int col, int row) {
            auto area2 = juce::Rectangle<int>(content.getX() + col * kw, content.getY() + row * kh, kw, kh);
            s.setBounds(area2.withTrimmedBottom(12));
            l.setBounds(area2.getX(), area2.getBottom() - 12, kw, 12);
        };
        placeKnob(reverbSizeSlider, rvSizeLabel, 0, 0);
        placeKnob(reverbDampSlider, rvDampLabel, 1, 0);
        placeKnob(reverbMixSlider, rvMixLabel, 0, 1);
        placeKnob(reverbWidthSlider, rvWidthLabel, 1, 1);
    }

    // Compressor layout
    {
        auto content = compSection.getContentArea().reduced(1);
        int kw = content.getWidth() / 3;
        int kh = content.getHeight() / 2;
        auto placeKnob = [&](juce::Slider& s, juce::Label& l, int col, int row) {
            auto area2 = juce::Rectangle<int>(content.getX() + col * kw, content.getY() + row * kh, kw, kh);
            s.setBounds(area2.withTrimmedBottom(12));
            l.setBounds(area2.getX(), area2.getBottom() - 12, kw, 12);
        };
        placeKnob(compThreshSlider, compThLabel, 0, 0);
        placeKnob(compRatioSlider, compRaLabel, 1, 0);
        placeKnob(compMakeupSlider, compMLabel, 2, 0);
        placeKnob(compAttackSlider, compALabel, 0, 1);
        placeKnob(compReleaseSlider, compRLabel, 1, 1);
    }

    // Chorus layout
    {
        auto content = chorusSection.getContentArea().reduced(2);
        int kw = content.getWidth() / 3;
        chorusRateSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        chorusDepthSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        chorusMixSlider.setBounds(content.withTrimmedBottom(12));
        chRateLabel.setBounds(chorusRateSlider.getX(), chorusRateSlider.getBottom() - 2, kw, 12);
        chDepthLabel.setBounds(chorusDepthSlider.getX(), chorusDepthSlider.getBottom() - 2, kw, 12);
        chMixLabel.setBounds(chorusMixSlider.getX(), chorusMixSlider.getBottom() - 2, content.getWidth(), 12);
    }

    main.removeFromTop(4);

    // Row 4: Phaser + EQ + Ring Mod + Waveshaper
    auto row4 = main.removeFromTop(rowHeight);
    int row4W = row4.getWidth();
    phaserSection.setBounds(row4.removeFromLeft(row4W / 4).reduced(2));
    eqSection.setBounds(row4.removeFromLeft(row4W / 4).reduced(2));
    ringModSection.setBounds(row4.removeFromLeft(row4W / 4).reduced(2));
    wsSection.setBounds(row4.reduced(2));

    // Phaser layout
    {
        auto content = phaserSection.getContentArea().reduced(2);
        int kw = content.getWidth() / 2;
        int kh = content.getHeight() / 2;
        auto placeKnob = [&](juce::Slider& s, juce::Label& l, int col, int row) {
            auto area2 = juce::Rectangle<int>(content.getX() + col * kw, content.getY() + row * kh, kw, kh);
            s.setBounds(area2.withTrimmedBottom(12));
            l.setBounds(area2.getX(), area2.getBottom() - 12, kw, 12);
        };
        placeKnob(phaserRateSlider, phRateLabel, 0, 0);
        placeKnob(phaserDepthSlider, phDepthLabel, 1, 0);
        placeKnob(phaserMixSlider, phMixLabel, 0, 1);
        placeKnob(phaserFbSlider, phFbLabel, 1, 1);
    }

    // EQ layout
    {
        auto content = eqSection.getContentArea().reduced(2);
        int kw = content.getWidth() / 2;
        int kh = content.getHeight() / 2;
        auto placeKnob = [&](juce::Slider& s, juce::Label& l, int col, int row) {
            auto area2 = juce::Rectangle<int>(content.getX() + col * kw, content.getY() + row * kh, kw, kh);
            s.setBounds(area2.withTrimmedBottom(12));
            l.setBounds(area2.getX(), area2.getBottom() - 12, kw, 12);
        };
        placeKnob(eqLowSlider, eqLowLabel, 0, 0);
        placeKnob(eqMidSlider, eqMidLabel, 1, 0);
        placeKnob(eqHighSlider, eqHighLabel, 0, 1);
        placeKnob(eqMidFreqSlider, eqFreqLabel, 1, 1);
    }

    // Ring Mod layout
    {
        auto content = ringModSection.getContentArea().reduced(2);
        int kw = content.getWidth() / 2;
        ringFreqSlider.setBounds(content.removeFromLeft(kw).withTrimmedBottom(12));
        ringMixSlider.setBounds(content.withTrimmedBottom(12));
        ringFreqLabel.setBounds(ringFreqSlider.getX(), ringFreqSlider.getBottom() - 2, kw, 12);
        ringMixLabel.setBounds(ringMixSlider.getX(), ringMixSlider.getBottom() - 2, content.getWidth(), 12);
    }

    // Waveshaper layout
    {
        auto content = wsSection.getContentArea().reduced(2);
        wsTypeBox.setBounds(content.removeFromTop(22).reduced(4, 0));
        content.removeFromTop(2);
        wsAmountSlider.setBounds(content.withTrimmedBottom(12));
        wsAmountLabel.setBounds(content.getX(), content.getBottom() - 12, content.getWidth(), 12);
    }
}
