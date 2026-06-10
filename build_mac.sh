#!/bin/bash
set -e

echo "========================================="
echo "  Schranz Machine - macOS Build & Install"
echo "========================================="
echo ""

# 1. Check prerequisites
echo "[1/6] Checking prerequisites..."

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
echo ""

# 2. Clone or update repo
echo "[2/6] Getting source code..."
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

# 3. Build (Universal Binary for Intel + Apple Silicon)
echo "[3/6] Building Universal Binary (this takes 3-5 minutes the first time)..."
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build --parallel "$(sysctl -n hw.ncpu)"
echo ""

# 4. Install VST3
echo "[4/6] Installing VST3 plugin..."
VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
mkdir -p "$VST3_DIR"
rm -rf "$VST3_DIR/Schranz Machine.vst3"
cp -R build/SchranzMachine_artefacts/Release/VST3/* "$VST3_DIR/"
echo "  VST3 -> $VST3_DIR/Schranz Machine.vst3"

# Remove quarantine attribute so macOS doesn't block it
xattr -dr com.apple.quarantine "$VST3_DIR/Schranz Machine.vst3" 2>/dev/null || true
echo ""

# 5. Create App in Applications with icon
echo "[5/6] Installing Standalone app..."
APP_NAME="Schranz Machine"
APP_SRC="build/SchranzMachine_artefacts/Release/Standalone/${APP_NAME}.app"

if [ -d "$APP_SRC" ]; then
    # Kill running instance if open (so we can overwrite)
    pkill -f "${APP_NAME}" 2>/dev/null || true
    sleep 0.5
    rm -rf "/Applications/${APP_NAME}.app"
    cp -R "$APP_SRC" "/Applications/${APP_NAME}.app"
    xattr -dr com.apple.quarantine "/Applications/${APP_NAME}.app" 2>/dev/null || true

    # Generate icon if possible
    if [ -f "Resources/icon.svg" ]; then
        ICONSET_DIR="/tmp/SchranzMachine.iconset"
        rm -rf "$ICONSET_DIR"
        mkdir -p "$ICONSET_DIR"

        # Try to convert SVG to ICNS via sips (macOS built-in)
        if command -v python3 &>/dev/null; then
            python3 Resources/create_icon.py 2>/dev/null || true
        fi

        # If we have a PNG, create iconset
        if [ -f "Resources/icon.png" ]; then
            for SIZE in 16 32 64 128 256 512; do
                sips -z $SIZE $SIZE "Resources/icon.png" --out "$ICONSET_DIR/icon_${SIZE}x${SIZE}.png" &>/dev/null || true
                DOUBLE=$((SIZE * 2))
                if [ $DOUBLE -le 1024 ]; then
                    sips -z $DOUBLE $DOUBLE "Resources/icon.png" --out "$ICONSET_DIR/icon_${SIZE}x${SIZE}@2x.png" &>/dev/null || true
                fi
            done
            iconutil -c icns "$ICONSET_DIR" -o "/Applications/${APP_NAME}.app/Contents/Resources/SchranzMachine.icns" 2>/dev/null || true
            # Update Info.plist to use our icon
            PLIST="/Applications/${APP_NAME}.app/Contents/Info.plist"
            if [ -f "$PLIST" ]; then
                /usr/libexec/PlistBuddy -c "Set :CFBundleIconFile SchranzMachine" "$PLIST" 2>/dev/null || true
            fi
        fi
        rm -rf "$ICONSET_DIR"
    fi

    echo "  App -> /Applications/${APP_NAME}.app"
fi
echo ""

# 6. Create/update Desktop alias (always refresh)
echo "[6/6] Updating Desktop shortcut..."
DESKTOP="$HOME/Desktop"
if [ -d "$DESKTOP" ]; then
    # Remove any old alias or symlink
    rm -f "$DESKTOP/${APP_NAME}"
    rm -f "$DESKTOP/${APP_NAME} alias"
    rm -rf "$DESKTOP/${APP_NAME}.app"

    # Create fresh alias via Finder
    osascript -e "tell application \"Finder\" to make alias file to POSIX file \"/Applications/${APP_NAME}.app\" at POSIX file \"$DESKTOP\"" 2>/dev/null || true
    echo "  Shortcut -> $DESKTOP/${APP_NAME}"

    # Refresh Finder so new icon shows immediately
    killall Finder 2>/dev/null || true
fi

echo ""
echo "========================================="
echo "  Installation complete!"
echo "========================================="
echo ""
echo "  Standalone: Double-click 'Schranz Machine' on Desktop"
echo "              or open /Applications/Schranz Machine.app"
echo ""
echo "  Ableton:    Restart Ableton -> Preferences -> Plug-Ins"
echo "              -> Rescan -> VST3 -> SchranzLab -> Schranz Machine"
echo ""
echo "  Features:   MIDI Keyboard (5 octaves), Drag & Drop Samples,"
echo "              2 Oscillators, Distortion, BitCrusher, Filter,"
echo "              Delay, Reverb, Compressor, Chorus, Phaser, EQ,"
echo "              Ring Mod, Waveshaper, 500+ Presets, 110+ MIDI Patterns"
echo ""
