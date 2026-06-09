#!/bin/bash
set -e

echo "========================================="
echo "  Schranz Machine - macOS Build & Install"
echo "========================================="
echo ""

# 1. Check prerequisites
echo "[1/5] Checking prerequisites..."

if ! command -v cmake &>/dev/null; then
    echo "CMake not found. Installing via Homebrew..."
    if ! command -v brew &>/dev/null; then
        echo "ERROR: Homebrew not installed."
        echo "Install it first: /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        exit 1
    fi
    brew install cmake
fi

if ! xcode-select -p &>/dev/null; then
    echo "Xcode Command Line Tools not found. Installing..."
    xcode-select --install
    echo "After installation finishes, run this script again."
    exit 1
fi

echo "  cmake: $(cmake --version | head -1)"
echo "  Xcode CLT: $(xcode-select -p)"
echo ""

# 2. Clone or update repo
echo "[2/5] Getting source code..."
INSTALL_DIR="$HOME/SchranzMachine"

if [ -d "$INSTALL_DIR/.git" ]; then
    echo "  Repository exists, updating..."
    cd "$INSTALL_DIR"
    git fetch origin
    git checkout claude/gracious-volta-l5l0dk
    git pull origin claude/gracious-volta-l5l0dk
else
    echo "  Cloning repository..."
    git clone https://github.com/pnzyyy/VST3Schranz.git "$INSTALL_DIR"
    cd "$INSTALL_DIR"
    git checkout claude/gracious-volta-l5l0dk
fi
echo ""

# 3. Build
echo "[3/5] Building (this takes 3-5 minutes the first time)..."
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(sysctl -n hw.ncpu)"
echo ""

# 4. Install VST3
echo "[4/5] Installing VST3 plugin..."
VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
mkdir -p "$VST3_DIR"
rm -rf "$VST3_DIR/Schranz Machine.vst3"
cp -r build/SchranzMachine_artefacts/Release/VST3/"Schranz Machine.vst3" "$VST3_DIR/"
echo "  VST3 -> $VST3_DIR/Schranz Machine.vst3"
echo ""

# 5. Done
echo "[5/5] Done!"
echo ""
echo "========================================="
echo "  Installation complete!"
echo "========================================="
echo ""
echo "  Standalone app:"
echo "    open $INSTALL_DIR/build/SchranzMachine_artefacts/Release/Standalone/Schranz\ Machine.app"
echo ""
echo "  VST3 in Ableton:"
echo "    Restart Ableton -> Preferences -> Plug-Ins -> Rescan"
echo "    Find it under: Instruments -> Plug-Ins -> VST3 -> SchranzLab"
echo ""
