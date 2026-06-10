cmake_minimum_required(VERSION 3.22)
project(SchranzMachine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SchranzMachine
    COMPANY_NAME "SchranzLab"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Shrz
    PLUGIN_CODE Sm01
    FORMATS VST3 Standalone
    PRODUCT_NAME "Schranz Machine"
    BUNDLE_ID "com.schranzlab.schranzmachine"
    VST3_CATEGORIES "Instrument" "Synth"
)

target_sources(SchranzMachine
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/DSP/Oscillator.cpp
        Source/DSP/Oscillator.h
        Source/DSP/SampleEngine.cpp
        Source/DSP/SampleEngine.h
        Source/DSP/DistortionEngine.cpp
        Source/DSP/DistortionEngine.h
        Source/DSP/FilterEngine.cpp
        Source/DSP/FilterEngine.h
        Source/DSP/SchranzVoice.cpp
        Source/DSP/SchranzVoice.h
        Source/DSP/SchranzSynth.cpp
        Source/DSP/SchranzSynth.h
        Source/DSP/EffectsEngine.cpp
        Source/DSP/EffectsEngine.h
        Source/Presets/PresetManager.cpp
        Source/Presets/PresetManager.h
        Source/Presets/FactoryPresets.h
)

juce_generate_juce_header(SchranzMachine)

target_compile_definitions(SchranzMachine
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(SchranzMachine
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# --- Packaging ---
# Use ./install.sh for local installation
# Use ./package.sh to create distributable packages (tar.gz + .deb)
