#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SchranzLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SchranzLookAndFeel()
    {
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xFF111111));
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFCC0000));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF333333));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xFFCC0000));
        setColour(juce::Label::textColourId, juce::Colour(0xFFCCCCCC));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF222222));
        setColour(juce::ComboBox::textColourId, juce::Colour(0xFFCCCCCC));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFF444444));
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF222222));
        setColour(juce::TextButton::textColourOffId, juce::Colour(0xFFCCCCCC));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override
    {
        auto radius = static_cast<float>(juce::jmin(width / 2, height / 2)) - 4.0f;
        auto centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
        auto centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Background arc
        juce::Path bgArc;
        bgArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                            rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colour(0xFF333333));
        g.strokePath(bgArc, juce::PathStrokeType(3.0f));

        // Value arc
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                               rotaryStartAngle, angle, true);
        g.setColour(juce::Colour(0xFFCC0000));
        g.strokePath(valueArc, juce::PathStrokeType(3.0f));

        // Dot
        juce::Point<float> thumbPoint(centreX + (radius - 6.0f) * std::cos(angle - juce::MathConstants<float>::halfPi),
                                       centreY + (radius - 6.0f) * std::sin(angle - juce::MathConstants<float>::halfPi));
        g.setColour(juce::Colours::white);
        g.fillEllipse(thumbPoint.x - 3.0f, thumbPoint.y - 3.0f, 6.0f, 6.0f);

        (void)slider;
    }
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

class SchranzMachineEditor : public juce::AudioProcessorEditor
{
public:
    explicit SchranzMachineEditor(SchranzMachineProcessor&);
    ~SchranzMachineEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SchranzMachineProcessor& processorRef;
    SchranzLookAndFeel schranzLnf;

    // Preset controls
    juce::TextButton prevPresetBtn{"<"};
    juce::TextButton nextPresetBtn{">"};
    juce::Label presetNameLabel;

    // Osc 1
    juce::ComboBox osc1TypeBox;
    juce::Slider osc1GainSlider;
    juce::Slider osc1DetuneSlider;
    juce::Label osc1Label{"", "OSC 1"};

    // Osc 2
    juce::ComboBox osc2TypeBox;
    juce::Slider osc2GainSlider;
    juce::Slider osc2DetuneSlider;
    juce::Label osc2Label{"", "OSC 2"};

    // Amp Envelope
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label envLabel{"", "ENVELOPE"};

    // Distortion
    juce::ComboBox distTypeBox;
    juce::Slider driveSlider, distMixSlider;
    juce::Label distLabel{"", "DISTORTION"};

    // Crusher
    juce::Slider crushBitsSlider, crushRateSlider;
    juce::Label crushLabel{"", "CRUSHER"};

    // Filter
    juce::ComboBox filterTypeBox;
    juce::Slider cutoffSlider, resonanceSlider, filterEnvSlider;
    juce::Label filterLabel{"", "FILTER"};

    // Master
    juce::Slider masterSlider;
    juce::Label masterLabel{"", "MASTER"};

    // Sample area
    DragDropArea dragDropArea;

    // Sample controls
    juce::ToggleButton sampleLoopBtn{"Loop"};

    // APVTS attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1TypeAttach, osc2TypeAttach, distTypeAttach, filterTypeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        osc1GainAttach, osc1DetuneAttach,
        osc2GainAttach, osc2DetuneAttach,
        attackAttach, decayAttach, sustainAttach, releaseAttach,
        driveAttach, distMixAttach,
        crushBitsAttach, crushRateAttach,
        cutoffAttach, resonanceAttach, filterEnvAttach,
        masterAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sampleLoopAttach;

    void setupSlider(juce::Slider& slider);
    void setupLabel(juce::Label& label);
    void updatePresetLabel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SchranzMachineEditor)
};
