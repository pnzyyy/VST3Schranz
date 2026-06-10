#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Presets/SuggestionEngine.h"

class SchranzLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SchranzLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float startAngle, float endAngle,
                          juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    static constexpr uint32_t kBgDark     = 0xFF0A0A0A;
    static constexpr uint32_t kBgPanel    = 0xFF151515;
    static constexpr uint32_t kBgSection  = 0xFF1C1C1C;
    static constexpr uint32_t kAccent     = 0xFFE01020;
    static constexpr uint32_t kAccentDim  = 0xFF8B0A14;
    static constexpr uint32_t kTextBright = 0xFFDDDDDD;
    static constexpr uint32_t kTextDim    = 0xFF777777;
    static constexpr uint32_t kBorder     = 0xFF2A2A2A;
};

class SectionPanel : public juce::Component
{
public:
    SectionPanel(const juce::String& title);
    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getContentArea() const;

private:
    juce::String title;
};

class DragDropArea : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    DragDropArea(SchranzMachineProcessor& p);
    void paint(juce::Graphics& g) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

private:
    SchranzMachineProcessor& processor;
    bool isDragOver = false;
};

class SchranzMachineEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit SchranzMachineEditor(SchranzMachineProcessor&);
    ~SchranzMachineEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // AI suggestions
    juce::OwnedArray<juce::TextButton> suggestionBtns;
    SectionPanel suggestSection{"AI SUGGESTIONS"};
    void refreshSuggestions();

private:
    void timerCallback() override;

    SchranzMachineProcessor& processorRef;
    SchranzLookAndFeel schranzLnf;

    // MIDI Keyboard
    juce::MidiKeyboardComponent keyboard;

    // Preset controls
    juce::TextButton prevPresetBtn{"<"};
    juce::TextButton nextPresetBtn{">"};
    juce::Label presetNameLabel;

    // Preset browser
    juce::ComboBox presetCategoryBox;
    juce::ComboBox presetNameBox;
    juce::Label presetCategoryLabel{"", "CATEGORY"};
    juce::Label presetBrowserLabel{"", "PRESET"};

    // MIDI Pattern rack
    juce::ComboBox patternCategoryBox;
    juce::ComboBox patternNameBox;
    juce::TextButton patternPlayBtn{"PLAY"};
    juce::Slider patternBpmSlider;
    juce::Label patternCategoryLabel{"", "MIDI"};
    juce::Label patternNameLabelUI{"", "PATTERN"};
    juce::Label patternBpmLabel{"", "BPM"};
    SectionPanel patternRackSection{"MIDI PATTERN RACK"};
    std::unique_ptr<SuggestionEngine> suggestionEngine;

    // Section panels
    SectionPanel oscSection{"OSCILLATORS"};
    SectionPanel sampleSection{"SAMPLE"};
    SectionPanel envSection{"ENVELOPE"};
    SectionPanel distSection{"DISTORTION"};
    SectionPanel crushSection{"CRUSHER"};
    SectionPanel filterSection{"FILTER"};
    SectionPanel delaySection{"DELAY"};
    SectionPanel reverbSection{"REVERB"};
    SectionPanel compSection{"COMPRESSOR"};
    SectionPanel chorusSection{"CHORUS"};
    SectionPanel phaserSection{"PHASER"};
    SectionPanel eqSection{"EQ"};
    SectionPanel ringModSection{"RING MOD"};
    SectionPanel wsSection{"WAVESHAPER"};

    // Osc 1
    juce::ComboBox osc1TypeBox, osc2TypeBox;
    juce::Slider osc1GainSlider, osc1DetuneSlider;
    juce::Slider osc2GainSlider, osc2DetuneSlider;
    juce::Label osc1GainLabel{"", "GAIN"}, osc1DetuneLabel{"", "DETUNE"};
    juce::Label osc2GainLabel{"", "GAIN"}, osc2DetuneLabel{"", "DETUNE"};
    juce::Label osc1TitleLabel{"", "OSC 1"}, osc2TitleLabel{"", "OSC 2"};

    // Amp Envelope
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackLabel{"", "A"}, decayLabel{"", "D"}, sustainLabel{"", "S"}, releaseLabel{"", "R"};

    // Distortion
    juce::ComboBox distTypeBox;
    juce::Slider driveSlider, distMixSlider;
    juce::Label driveLabel{"", "DRIVE"}, distMixLabel{"", "MIX"};

    // Crusher
    juce::Slider crushBitsSlider, crushRateSlider;
    juce::Label crushBitsLabel{"", "BITS"}, crushRateLabel{"", "RATE"};

    // Filter
    juce::ComboBox filterTypeBox;
    juce::Slider cutoffSlider, resonanceSlider, filterEnvSlider;
    juce::Label cutoffLabel{"", "CUTOFF"}, resLabel{"", "RES"}, fEnvLabel{"", "ENV"};

    // Delay
    juce::Slider delayTimeSlider, delayFbSlider, delayMixSlider;
    juce::ToggleButton delayPPBtn{"PP"};
    juce::Label delayTimeLabel{"", "TIME"}, delayFbLabel{"", "FB"}, delayMixLabel{"", "MIX"};

    // Reverb
    juce::Slider reverbSizeSlider, reverbDampSlider, reverbMixSlider, reverbWidthSlider;
    juce::Label rvSizeLabel{"", "SIZE"}, rvDampLabel{"", "DAMP"}, rvMixLabel{"", "MIX"}, rvWidthLabel{"", "WIDTH"};

    // Compressor
    juce::Slider compThreshSlider, compRatioSlider, compAttackSlider, compReleaseSlider, compMakeupSlider;
    juce::Label compThLabel{"", "THRESH"}, compRaLabel{"", "RATIO"}, compALabel{"", "ATK"}, compRLabel{"", "REL"}, compMLabel{"", "GAIN"};

    // Chorus
    juce::Slider chorusRateSlider, chorusDepthSlider, chorusMixSlider;
    juce::Label chRateLabel{"", "RATE"}, chDepthLabel{"", "DEPTH"}, chMixLabel{"", "MIX"};

    // Phaser
    juce::Slider phaserRateSlider, phaserDepthSlider, phaserMixSlider, phaserFbSlider;
    juce::Label phRateLabel{"", "RATE"}, phDepthLabel{"", "DEPTH"}, phMixLabel{"", "MIX"}, phFbLabel{"", "FB"};

    // EQ
    juce::Slider eqLowSlider, eqMidSlider, eqHighSlider, eqMidFreqSlider;
    juce::Label eqLowLabel{"", "LOW"}, eqMidLabel{"", "MID"}, eqHighLabel{"", "HIGH"}, eqFreqLabel{"", "FREQ"};

    // Ring Mod
    juce::Slider ringFreqSlider, ringMixSlider;
    juce::Label ringFreqLabel{"", "FREQ"}, ringMixLabel{"", "MIX"};

    // Waveshaper
    juce::ComboBox wsTypeBox;
    juce::Slider wsAmountSlider;
    juce::Label wsAmountLabel{"", "AMOUNT"};

    // Master
    juce::Slider masterSlider;

    // Sample
    DragDropArea dragDropArea;
    juce::ToggleButton sampleLoopBtn{"Loop"};

    // APVTS attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        osc1TypeAttach, osc2TypeAttach, distTypeAttach, filterTypeAttach, wsTypeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        osc1GainAttach, osc1DetuneAttach, osc2GainAttach, osc2DetuneAttach,
        attackAttach, decayAttach, sustainAttach, releaseAttach,
        driveAttach, distMixAttach, crushBitsAttach, crushRateAttach,
        cutoffAttach, resonanceAttach, filterEnvAttach,
        delayTimeAttach, delayFbAttach, delayMixAttach,
        rvSizeAttach, rvDampAttach, rvMixAttach, rvWidthAttach,
        compThreshAttach, compRatioAttach, compAttackAttach, compReleaseAttach, compMakeupAttach,
        chRateAttach, chDepthAttach, chMixAttach,
        phRateAttach, phDepthAttach, phMixAttach, phFbAttach,
        eqLowAttach, eqMidAttach, eqHighAttach, eqMidFreqAttach,
        ringFreqAttach, ringMixAttach,
        wsAmountAttach,
        masterAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        sampleLoopAttach, delayPPAttach;

    void setupKnob(juce::Slider& slider);
    void setupKnobLabel(juce::Label& label);
    void setupSectionLabel(juce::Label& label);
    void updatePresetLabel();
    void refreshPresetCategoryList();
    void refreshPresetNameList();
    void refreshPatternCategoryList();
    void refreshPatternNameList();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SchranzMachineEditor)
};
