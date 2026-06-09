#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
ARTEFACTS="$BUILD_DIR/SchranzMachine_artefacts"

if [ ! -d "$ARTEFACTS" ]; then
    echo "Build not found. Building first..."
    cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release "$SCRIPT_DIR"
    cmake --build "$BUILD_DIR" --parallel "$(nproc 2>/dev/null || echo 4)"
fi

# Detect build config subdirectory
if [ -d "$ARTEFACTS/Release" ]; then
    ARTEFACTS="$ARTEFACTS/Release"
fi

# Install VST3
case "$(uname -s)" in
    Linux*)
        VST3_DIR="$HOME/.vst3"
        BIN_DIR="$HOME/.local/bin"
        ;;
    Darwin*)
        VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
        BIN_DIR="/usr/local/bin"
        ;;
    MINGW*|MSYS*|CYGWIN*)
        VST3_DIR="$COMMONPROGRAMFILES/VST3"
        BIN_DIR="$PROGRAMFILES/SchranzMachine"
        ;;
esac

echo "Installing Schranz Machine..."
echo ""

# VST3 plugin
if [ -d "$ARTEFACTS/VST3" ]; then
    mkdir -p "$VST3_DIR"
    cp -r "$ARTEFACTS/VST3/"* "$VST3_DIR/"
    echo "  VST3 installed to: $VST3_DIR"
else
    echo "  Warning: VST3 build not found"
fi

# Standalone
STANDALONE="$ARTEFACTS/Standalone/Schranz Machine"
if [ -f "$STANDALONE" ]; then
    mkdir -p "$BIN_DIR"
    cp "$STANDALONE" "$BIN_DIR/"
    chmod +x "$BIN_DIR/Schranz Machine"
    echo "  Standalone installed to: $BIN_DIR/Schranz Machine"
else
    echo "  Warning: Standalone build not found"
fi

echo ""
echo "Installation complete! Restart your DAW to scan for the new plugin."
