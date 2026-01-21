# Keyball39

Keyball39 is a custom keyboard configuration designed to optimize typing efficiency, especially for tasks requiring frequent use of brackets and other text input symbols.

## Features
- Customize layers for various input modes:
  - Base Layer (Layer 0): Standard layout with frequent symbols.
  - Lower Layer (Layer 1): Access to numbers, brackets, and navigation keys.
  - Settings Layer (Layer 2): Function keys, trackball settings, and macros.
- Includes advanced key combinations for improved productivity:
  - Example: `Q + W`: Switch to Layer 2 while pressed.
- OLED Display Support: Key input, layer, and trackball information displayed on OLED screens.

## Getting Started
To use this keymap with your Keyball39 keyboard:
1. Clone this repository.
2. Replace your existing `keymap.c` with the one provided here.
3. Compile and flash your firmware using QMK.

## Key Features Breakdown
### Combo Keys
- Enter: `H + J`
- Switch to Layer 2: `Q + W`

### Symbol Frequency Design
- Ultra-frequent: `{ } ( ) [ ] : = -`
- Frequent: `_ " ' ; / \
- Semi-frequent: `` ` ~ #
- Home rows optimized for bracket input.

## Prerequisites
You need QMK setup to flash this configuration onto your Keyball39 keyboard.

```bash
qmk flash -kb keyball39 -km your_keymap
```

Feel free to adapt this keymap for your specific workflow!