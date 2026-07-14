# ColorEstimation Class

A versatile C++ color manipulation utility class for pixel processing, color prediction, and artistic tone transformations.

## 📋 Overview

`ColorEstimation` provides a comprehensive set of tools for working with RGB colors, including alpha estimation, color compositing, sequence prediction, and 24 distinct artistic color tone presets. All color values use 8-bit channels (0-255).

## 🏗️ Structure

### Data Types

#### `RGB` struct
```cpp
struct RGB {
    uint8_t r, g, b;  // Red, Green, Blue (0-255)
};
```

#### `Tone` enum
24 preset color transformation styles:
- **Atmospheric**: SOFT, INTENSE, PASTEL, MUTED, ROUGH
- **Temperature**: WARM, COLD, EMBER, FROST, HAZE
- **Environmental**: SUNSET, OCEAN, FOREST, TWILIGHT
- **Stylistic**: GOLDEN, NOIR, NEON, DREAM, CANDY, LAVA
- **Thematic**: ALIEN, VINTAGE, CYBER

## 🔧 Functions

### `estimateAlpha()`
**Estimates opacity of a color against a background**

```cpp
uint8_t estimateAlpha(uint8_t r, uint8_t g, uint8_t b, 
                      uint8_t bgR, uint8_t bgG, uint8_t bgB)
```

Combines Euclidean distance (60%) and perceptual luminance difference (40%) to calculate alpha.

**Returns:** Alpha value (0-255)

**Example:**
```cpp
uint8_t alpha = ce.estimateAlpha(255, 0, 0, 255, 255, 255);
// Red text on white background → alpha: 255 (fully opaque)
```

---

### `rgbaToRgb()`
**Converts RGBA to RGB with alpha compositing**

```cpp
RGB rgbaToRgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a,
              uint8_t bgR, uint8_t bgG, uint8_t bgB)
```

Standard "over" compositing operation blending foreground with background.

**Returns:** Composited RGB color

**Example:**
```cpp
auto result = ce.rgbaToRgb(255, 0, 0, 128, 255, 255, 255);
// 50% red on white → RGB(255, 128, 128)
```

---

### `nextPixel()`
**Predicts the next color in a sequence**

```cpp
RGB nextPixel(std::vector<RGB> &history)
```

Uses velocity and weighted acceleration from color history:
- **0-1 pixels**: Returns black (0,0,0)
- **2 pixels**: Linear extrapolation
- **3+ pixels**: Weighted acceleration model (recent changes weighted more)

**Returns:** Predicted next RGB color

**Example:**
```cpp
std::vector<colorEstimation::RGB> history = {
    {100, 50, 200}, {120, 55, 195}, {145, 62, 188}
};
auto next = ce.nextPixel(history);
// Predicts acceleration trend → RGB(175, 71, 179)
```

---

### `middlePixel()`
**Calculates the average of two colors**

```cpp
RGB middlePixel(RGB leftPixel, RGB rightPixel)
```

Simple per-channel arithmetic mean.

**Returns:** Midpoint RGB color

**Example:**
```cpp
auto mid = ce.middlePixel({255, 0, 0}, {0, 0, 255});
// Red + Blue → RGB(127, 0, 127) [purple]
```

---

### `highlightPixel()`
**Adjusts pixel brightness intensity**

```cpp
RGB highlightPixel(std::vector<RGB> &history, RGB pixel, float intensity)
```

Intensity mapping:
- **0%**: Fully black
- **50%**: Original color
- **100%**: Fully white

Modifies the pixel in the history vector in-place.

**Parameters:**
- `intensity`: Float 0.0-100.0

**Returns:** Modified RGB color

**Example:**
```cpp
std::vector<colorEstimation::RGB> history = {{128, 128, 128}};
auto result = ce.highlightPixel(history, {128, 128, 128}, 25.0f);
// 25% intensity → RGB(64, 64, 64)
```

---

### `getRandomPixel()`
**Generates a pseudo-random color**

```cpp
RGB getRandomPixel()
```

Uses xorshift32 PRNG algorithm seeded with `123456789`.

**Returns:** Random RGB color

**Example:**
```cpp
auto pixel = ce.getRandomPixel();
// Random color → RGB(42, 151, 184)
```

---

### `tonePixels()`
**Applies artistic color tone transformations**

```cpp
std::vector<RGB> tonePixels(std::vector<RGB> &history, Tone tone, float strength)
```

Transforms all pixels in a vector using complex per-channel mathematical operations based on the selected tone preset.

**Parameters:**
- `tone`: One of 24 `Tone` enum values
- `strength`: Float 0.0-100.0 (effect intensity)

**Returns:** Vector of transformed RGB colors

**Example:**
```cpp
std::vector<colorEstimation::RGB> pixels = {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255}
};
auto warm = ce.tonePixels(pixels, colorEstimation::Tone::WARM, 50.0f);
auto cyber = ce.tonePixels(pixels, colorEstimation::Tone::CYBER, 80.0f);
```

## 🎨 Tone Preset Gallery

| Tone | Effect Description |
|------|-------------------|
| **SOFT** | Gentle desaturation, moves toward pastel |
| **INTENSE** | Boosts saturation and contrast |
| **WARM** | Adds red/yellow warmth, reduces blue |
| **COLD** | Adds blue coolness, reduces red |
| **PASTEL** | Lightens and softens colors |
| **MUTED** | Desaturates toward gray |
| **VIBRANT** | Amplifies dominant channel to maximum |
| **ROUGH** | Adds random noise and texture |
| **GOLDEN** | Golden hour warmth, reduces blues |
| **NOIR** | Film noir style, high contrast grayscale |
| **NEON** | Electric, glowing color boost |
| **DREAM** | Soft, ethereal with color bleeding |
| **SUNSET** | Orange-red warmth with purple undertones |
| **OCEAN** | Deep blue-green aquatic tones |
| **FOREST** | Earthy green-dominant natural tones |
| **TWILIGHT** | Purple-blue evening darkness |
| **EMBER** | Fiery red-orange, suppresses cool tones |
| **FROST** | Icy blue-white winter atmosphere |
| **HAZE** | Foggy, muted atmospheric effect |
| **CANDY** | Bright, sweet pastel-like colors |
| **LAVA** | Intense red-orange volcanic glow |
| **ALIEN** | Unnatural green-purple sci-fi tones |
| **VINTAGE** | Aged, sepia-like retro appearance |
| **CYBER** | High-tech blue-purple neon aesthetic |

## 📊 Technical Details

### Color Mathematics
- **Luminance formula**: `0.2126R + 0.7152G + 0.0722B` (ITU-R BT.709)
- **Alpha blending**: Standard `foreground * alpha + background * (1 - alpha)`
- **Maximum RGB distance**: `√(255² + 255² + 255²) ≈ 441.673`
- **Sequence prediction**: Uses weighted acceleration with linearly increasing weights

### Performance Considerations
- No dynamic memory allocation in core functions
- Static PRNG state persists across calls
- Clamped output values prevent overflow
- O(n) complexity for history-based operations

## 📝 Usage Examples

### Basic Color Processing
```cpp
colorEstimation ce;

// Blend semi-transparent overlay
auto blended = ce.rgbaToRgb(255, 255, 0, 180, 50, 50, 50);

// Find color midpoint
auto midpoint = ce.middlePixel({100, 200, 50}, {200, 100, 150});
```

### Sequence Prediction
```cpp
std::vector<colorEstimation::RGB> colorHistory;
// ... add colors to history
auto predicted = ce.nextPixel(colorHistory);
```

### Artistic Tone Application
```cpp
std::vector<colorEstimation::RGB> image;
// ... load image data
auto cyberpunk = ce.tonePixels(image, colorEstimation::Tone::CYBER, 75.0f);
auto vintage = ce.tonePixels(image, colorEstimation::Tone::VINTAGE, 60.0f);
```

## 📜 License

This code is provided for educational and creative purposes. Feel free to use, modify, and distribute.