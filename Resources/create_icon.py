#!/usr/bin/env python3
"""Generate Schranz Machine icon as SVG and PNG (if pillow available)."""
import os

SVG_ICON = '''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
  <defs>
    <radialGradient id="bg" cx="50%" cy="40%" r="60%">
      <stop offset="0%" style="stop-color:#1a1a1a"/>
      <stop offset="100%" style="stop-color:#050505"/>
    </radialGradient>
    <linearGradient id="redGlow" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#ff2030"/>
      <stop offset="100%" style="stop-color:#8b0a14"/>
    </linearGradient>
  </defs>

  <!-- Background -->
  <rect width="512" height="512" rx="80" fill="url(#bg)"/>
  <rect x="4" y="4" width="504" height="504" rx="78" fill="none" stroke="#e01020" stroke-width="3" opacity="0.6"/>

  <!-- Waveform -->
  <path d="M 80,256 L 140,160 160,340 180,140 200,360 220,120 240,380 260,100 280,400 300,120 320,380 340,140 360,340 380,160 400,300 432,256"
        fill="none" stroke="url(#redGlow)" stroke-width="4" stroke-linecap="round" stroke-linejoin="round" opacity="0.9"/>

  <!-- Glow behind waveform -->
  <path d="M 80,256 L 140,160 160,340 180,140 200,360 220,120 240,380 260,100 280,400 300,120 320,380 340,140 360,340 380,160 400,300 432,256"
        fill="none" stroke="#e01020" stroke-width="12" stroke-linecap="round" stroke-linejoin="round" opacity="0.15"/>

  <!-- S letter -->
  <text x="256" y="460" text-anchor="middle" font-family="Arial Black, Impact, sans-serif"
        font-size="80" font-weight="900" fill="#e01020" opacity="0.85">SM</text>

  <!-- Top label -->
  <text x="256" y="75" text-anchor="middle" font-family="Arial, sans-serif"
        font-size="28" font-weight="bold" fill="#666666" letter-spacing="8">SCHRANZ</text>
</svg>'''

script_dir = os.path.dirname(os.path.abspath(__file__))
svg_path = os.path.join(script_dir, "icon.svg")

with open(svg_path, "w") as f:
    f.write(SVG_ICON)
print(f"Created: {svg_path}")

try:
    from PIL import Image
    import cairosvg
    png_path = os.path.join(script_dir, "icon.png")
    cairosvg.svg2png(bytestring=SVG_ICON.encode(), write_to=png_path, output_width=512, output_height=512)
    print(f"Created: {png_path}")
except ImportError:
    print("Note: Install pillow+cairosvg for PNG export, or convert SVG manually")
    print("  brew install cairo && pip3 install cairosvg pillow")
