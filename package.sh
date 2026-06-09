#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
VERSION="1.0.0"
PKG_NAME="SchranzMachine-${VERSION}-$(uname -s)-$(uname -m)"
STAGING="/tmp/${PKG_NAME}"

# Detect build config subdirectory
ARTEFACTS="$BUILD_DIR/SchranzMachine_artefacts"
if [ -d "$ARTEFACTS/Release" ]; then
    ARTEFACTS="$ARTEFACTS/Release"
fi

if [ ! -d "$ARTEFACTS/VST3" ]; then
    echo "Build artefacts not found. Build first:"
    echo "  cmake -B build -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build build --parallel"
    exit 1
fi

echo "Packaging Schranz Machine ${VERSION}..."

# Clean staging
rm -rf "$STAGING"
mkdir -p "$STAGING"

# Copy artefacts
cp -r "$ARTEFACTS/VST3" "$STAGING/vst3"
if [ -d "$ARTEFACTS/Standalone" ]; then
    mkdir -p "$STAGING/bin"
    cp "$ARTEFACTS/Standalone/Schranz Machine" "$STAGING/bin/" 2>/dev/null || true
fi
cp "$SCRIPT_DIR/LICENSE" "$STAGING/"
cp "$SCRIPT_DIR/install.sh" "$STAGING/"

# Create tar.gz
cd /tmp
tar czf "$SCRIPT_DIR/build/${PKG_NAME}.tar.gz" "$PKG_NAME"

# Create DEB if dpkg-deb is available
if command -v dpkg-deb &>/dev/null; then
    DEB_DIR="/tmp/${PKG_NAME}-deb"
    rm -rf "$DEB_DIR"
    mkdir -p "$DEB_DIR/DEBIAN"
    mkdir -p "$DEB_DIR/usr/lib/vst3"
    mkdir -p "$DEB_DIR/usr/bin"

    cp -r "$ARTEFACTS/VST3/"* "$DEB_DIR/usr/lib/vst3/"
    if [ -f "$ARTEFACTS/Standalone/Schranz Machine" ]; then
        cp "$ARTEFACTS/Standalone/Schranz Machine" "$DEB_DIR/usr/bin/schranz-machine"
        chmod 755 "$DEB_DIR/usr/bin/schranz-machine"
    fi

    cat > "$DEB_DIR/DEBIAN/control" <<CTRL
Package: schranz-machine
Version: ${VERSION}
Section: sound
Priority: optional
Architecture: $(dpkg --print-architecture)
Depends: libasound2, libfreetype6, libx11-6, libxrandr2, libxinerama1, libxcursor1, libgl1
Maintainer: SchranzLab <schranzlab@example.com>
Description: Schranz Machine - VST3 Synthesizer
 A VST3 synthesizer and sampler plugin for Schranz and
 Industrial Techno music production. Features multiple
 oscillators, drag & drop sampling, distortion, bit crushing,
 resonant filter, and 12 factory presets.
CTRL

    cat > "$DEB_DIR/DEBIAN/postinst" <<'POST'
#!/bin/bash
# Symlink VST3 to user-scannable location
VST3_SYS="/usr/lib/vst3/Schranz Machine.vst3"
if [ -d "$VST3_SYS" ]; then
    echo "Schranz Machine installed. Restart your DAW to scan for new plugins."
fi
POST
    chmod 755 "$DEB_DIR/DEBIAN/postinst"

    dpkg-deb --build "$DEB_DIR" "$SCRIPT_DIR/build/${PKG_NAME}.deb"
    rm -rf "$DEB_DIR"
    echo "  DEB: build/${PKG_NAME}.deb"
fi

# Cleanup
rm -rf "$STAGING"

echo ""
echo "Packages created:"
echo "  TAR: build/${PKG_NAME}.tar.gz"
echo ""
echo "To install from tar.gz:"
echo "  tar xzf ${PKG_NAME}.tar.gz"
echo "  cd ${PKG_NAME}"
echo "  ./install.sh"
