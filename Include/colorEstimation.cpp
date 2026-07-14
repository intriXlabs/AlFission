#include <iostream>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>


class colorEstimation{
private:
public:
    enum Tone{
    SOFT, INTENSE, WARM, COLD, PASTEL, MUTED, VIBRANT, ROUGH,
    GOLDEN, NOIR, NEON, DREAM, SUNSET, OCEAN, FOREST, TWILIGHT,
    EMBER, FROST, HAZE, CANDY, LAVA, ALIEN, VINTAGE, CYBER
    };

    struct RGB{
        uint8_t r,g,b;
    };
    /**
     * Estimates the opacity (alpha) of an RGB color relative to
     * an arbitrary background color.
     *
     * The estimation combines:
     *   - Euclidean RGB distance
     *   - Perceptual luminance difference
     *
     * Returns alpha in the range 0-255.
     */
    uint8_t estimateAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t bgR, uint8_t bgG, uint8_t bgB){
        constexpr float MAX_DIST = 441.67295593f;

        float dr = (float)r - bgR;
        float dg = (float)g - bgG;
        float db = (float)b - bgB;
        float dist = std::sqrt(dr*dr + dg*dg + db*db) / MAX_DIST;

        float lum   = 0.2126f*r + 0.7152f*g + 0.0722f*b;
        float bgLum = 0.2126f*bgR + 0.7152f*bgG + 0.0722f*bgB;
        float alpha = 0.6f*dist + 0.4f*(std::abs(lum - bgLum) / 255.0f);

        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;

        return (uint8_t)(alpha * 255.0f + 0.5f);
    }

    RGB rgbaToRgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t bgR, uint8_t bgG, uint8_t bgB){
        float alpha = a / 255.0f;
        float inv   = 1.0f - alpha;

        return {
            static_cast<uint8_t>(r*alpha + bgR*inv + 0.5f),
            static_cast<uint8_t>(g*alpha + bgG*inv + 0.5f),
            static_cast<uint8_t>(b*alpha + bgB*inv + 0.5f)
        };
    }

    RGB nextPixel(std::vector<RGB> &history) {
        int n = history.size();

        if (n < 2) return {0, 0, 0};
        if (n == 2) {
            float r = history[1].r + (float)(history[1].r - history[0].r);
            float g = history[1].g + (float)(history[1].g - history[0].g);
            float b = history[1].b + (float)(history[1].b - history[0].b);
            return {
                (uint8_t)(r < 0 ? 0 : r > 255 ? 255 : r),
                (uint8_t)(g < 0 ? 0 : g > 255 ? 255 : g),
                (uint8_t)(b < 0 ? 0 : b > 255 ? 255 : b)
            };
        }

        float vR_prev = (float)(history[1].r - history[0].r);
        float vG_prev = (float)(history[1].g - history[0].g);
        float vB_prev = (float)(history[1].b - history[0].b);

        float aR_sum = 0, aG_sum = 0, aB_sum = 0;
        float weightSum = 0;
        float lastVR = 0, lastVG = 0, lastVB = 0;
        int w = 1;

        for (int i = 2; i < n; i++) {
            float vR = (float)(history[i].r - history[i-1].r);
            float vG = (float)(history[i].g - history[i-1].g);
            float vB = (float)(history[i].b - history[i-1].b);

            aR_sum += (vR - vR_prev) * w;
            aG_sum += (vG - vG_prev) * w;
            aB_sum += (vB - vB_prev) * w;
            weightSum += w;
            w++;

            vR_prev = vR;
            vG_prev = vG;
            vB_prev = vB;
        }

        lastVR = vR_prev;
        lastVG = vG_prev;
        lastVB = vB_prev;

        float accelR = aR_sum / weightSum;
        float accelG = aG_sum / weightSum;
        float accelB = aB_sum / weightSum;

        float r = history[n-1].r + lastVR + accelR;
        float g = history[n-1].g + lastVG + accelG;
        float b = history[n-1].b + lastVB + accelB;

        return {
            static_cast<uint8_t>(r < 0 ? 0 : r > 255 ? 255 : r),
            static_cast<uint8_t>(g < 0 ? 0 : g > 255 ? 255 : g),
            static_cast<uint8_t>(b < 0 ? 0 : b > 255 ? 255 : b)
        };
    }

    RGB middlePixel(RGB leftPixel, RGB rightPixel) {
        return {
            static_cast<uint8_t>((leftPixel.r + rightPixel.r) / 2),
            static_cast<uint8_t>((leftPixel.g + rightPixel.g) / 2),
            static_cast<uint8_t>((leftPixel.b + rightPixel.b) / 2)
        };
    }

    RGB highlightPixel(std::vector<RGB> &history, RGB pixel, float intensity) {
        if (intensity < 0.0f || intensity > 100.0f) return pixel;
        
        float t = intensity / 100.0f; // 0.0 to 1.0
        
        // 0% -> darkest (black)
        // 50% -> original pixel
        // 100% -> lightest (white)
        
        for (int i = 0; i < history.size(); i++) {
            if (history[i].r == pixel.r && history[i].g == pixel.g && history[i].b == pixel.b) {
                if (t <= 0.5f) {
                    float s = t * 2.0f; // 0 to 1
                    history[i].r = (uint8_t)(pixel.r * s);
                    history[i].g = (uint8_t)(pixel.g * s);
                    history[i].b = (uint8_t)(pixel.b * s);
                } else {
                    float s = (t - 0.5f) * 2.0f; // 0 to 1
                    history[i].r = (uint8_t)(pixel.r + (255 - pixel.r) * s);
                    history[i].g = (uint8_t)(pixel.g + (255 - pixel.g) * s);
                    history[i].b = (uint8_t)(pixel.b + (255 - pixel.b) * s);
                }
                
                return history[i];
            }
        }
        return pixel;
    }

    RGB getRandomPixel() {
        static uint32_t seed = 123456789;
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        
        return {
            (uint8_t)(seed % 256),
            (uint8_t)((seed >> 8) % 256),
            (uint8_t)((seed >> 16) % 256)
        };
    }

    std::vector<RGB> tonePixels(std::vector<RGB> &history, Tone tone, float strength) {
        std::vector<RGB> result(history.size());
        float s = strength / 100.0f;
        
        for (int i = 0; i < history.size(); i++) {
            float r = history[i].r;
            float g = history[i].g;
            float b = history[i].b;
            float gray = 0.2126f*r + 0.7152f*g + 0.0722f*b;
            float tr, tg, tb;
            
            switch (tone) {
                case Tone::SOFT:
                    tr = r + (gray - r) * s * 0.5f + (128.0f - r) * s * 0.3f;
                    tg = g + (gray - g) * s * 0.5f + (128.0f - g) * s * 0.3f;
                    tb = b + (gray - b) * s * 0.5f + (128.0f - b) * s * 0.3f;
                    break;
                case Tone::INTENSE:
                    tr = r + (r - gray) * s * 0.8f + (r - 128.0f) * s * 0.6f;
                    tg = g + (g - gray) * s * 0.8f + (g - 128.0f) * s * 0.6f;
                    tb = b + (b - gray) * s * 0.8f + (b - 128.0f) * s * 0.6f;
                    break;
                case Tone::WARM:
                    tr = r + (255.0f - r) * s * 0.4f + (r - gray) * s * 0.2f;
                    tg = g + (255.0f - g) * s * 0.15f + (gray - g) * s * 0.1f;
                    tb = b - b * s * 0.5f + (gray - b) * s * 0.2f;
                    break;
                case Tone::COLD:
                    tr = r - r * s * 0.5f + (gray - r) * s * 0.2f;
                    tg = g + (255.0f - g) * s * 0.15f + (gray - g) * s * 0.1f;
                    tb = b + (255.0f - b) * s * 0.4f + (b - gray) * s * 0.2f;
                    break;
                case Tone::PASTEL:
                    tr = r + (255.0f - r) * s * 0.7f + (gray - r) * s * 0.2f;
                    tg = g + (255.0f - g) * s * 0.7f + (gray - g) * s * 0.2f;
                    tb = b + (255.0f - b) * s * 0.7f + (gray - b) * s * 0.2f;
                    break;
                case Tone::MUTED:
                    tr = r + (gray - r) * s * 0.9f;
                    tg = g + (gray - g) * s * 0.9f;
                    tb = b + (gray - b) * s * 0.9f;
                    break;
                case Tone::VIBRANT: {
                    float maxC = r > g ? (r > b ? r : b) : (g > b ? g : b);
                    tr = r + ((r == maxC ? 255.0f : 0) - r) * s * 0.7f + (r - gray) * s * 0.5f;
                    tg = g + ((g == maxC ? 255.0f : 0) - g) * s * 0.7f + (g - gray) * s * 0.5f;
                    tb = b + ((b == maxC ? 255.0f : 0) - b) * s * 0.7f + (b - gray) * s * 0.5f;
                    break;
                }
                case Tone::ROUGH: {
                    static uint32_t seed = 1;
                    seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
                    float nr = ((seed % 80) - 40) * s;
                    float ng = ((seed % 80) - 40) * s;
                    float nb = ((seed % 80) - 40) * s;
                    tr = r + nr + (r - gray) * s * 0.3f;
                    tg = g + ng + (g - gray) * s * 0.3f;
                    tb = b + nb + (b - gray) * s * 0.3f;
                    break;
                }
                case Tone::GOLDEN:
                    tr = r + (255.0f - r) * s * 0.5f + (r - b) * s * 0.3f;
                    tg = g + (215.0f - g) * s * 0.4f + (g - b * 0.5f) * s * 0.3f;
                    tb = b - b * s * 0.6f + (r * 0.2f - b) * s * 0.3f;
                    break;
                case Tone::NOIR: {
                    float bw = gray;
                    tr = bw + (r - bw) * (1.0f - s) + (bw - 128.0f) * s * 0.4f;
                    tg = bw + (g - bw) * (1.0f - s) + (bw - 128.0f) * s * 0.4f;
                    tb = bw + (b - bw) * (1.0f - s) + (bw - 128.0f) * s * 0.4f;
                    break;
                }
                case Tone::NEON:
                    tr = r + (255.0f - r) * s * 0.8f + (r - gray) * s * 0.6f;
                    tg = g + (255.0f - g) * s * 0.3f + (g - b) * s * 0.5f;
                    tb = b + (255.0f - b) * s * 0.8f + (b - r) * s * 0.5f;
                    break;
                case Tone::DREAM:
                    tr = r + (255.0f - r) * s * 0.4f + (g - r) * s * 0.3f + (b - r) * s * 0.2f;
                    tg = g + (255.0f - g) * s * 0.4f + (r - g) * s * 0.3f + (b - g) * s * 0.2f;
                    tb = b + (255.0f - b) * s * 0.4f + (r - b) * s * 0.3f + (g - b) * s * 0.2f;
                    break;
                case Tone::SUNSET:
                    tr = r + (255.0f - r) * s * 0.6f;
                    tg = g + (140.0f - g) * s * 0.5f + (r - g) * s * 0.3f;
                    tb = b + (180.0f - b) * s * 0.4f - b * s * 0.3f;
                    break;
                case Tone::OCEAN:
                    tr = r - r * s * 0.5f + (b * 0.2f - r) * s * 0.3f;
                    tg = g + (180.0f - g) * s * 0.4f + (b - g) * s * 0.3f;
                    tb = b + (255.0f - b) * s * 0.5f + (b - r) * s * 0.3f;
                    break;
                case Tone::FOREST:
                    tr = r - r * s * 0.5f + (g * 0.1f - r) * s * 0.3f;
                    tg = g + (200.0f - g) * s * 0.4f + (g - r) * s * 0.3f + (g - b) * s * 0.2f;
                    tb = b - b * s * 0.4f + (g * 0.1f - b) * s * 0.2f;
                    break;
                case Tone::TWILIGHT:
                    tr = r + (100.0f - r) * s * 0.5f + (b - r) * s * 0.3f;
                    tg = g + (80.0f - g) * s * 0.4f + (b * 0.3f - g) * s * 0.2f;
                    tb = b + (180.0f - b) * s * 0.5f + (b - r) * s * 0.3f;
                    break;
                case Tone::EMBER:
                    tr = r + (255.0f - r) * s * 0.7f;
                    tg = g + (100.0f - g) * s * 0.5f + (r * 0.3f - g) * s * 0.3f;
                    tb = b - b * s * 0.7f + (r * 0.05f - b) * s * 0.2f;
                    break;
                case Tone::FROST:
                    tr = r + (220.0f - r) * s * 0.4f + (b - r) * s * 0.3f;
                    tg = g + (230.0f - g) * s * 0.4f + (b - g) * s * 0.3f;
                    tb = b + (255.0f - b) * s * 0.5f;
                    break;
                case Tone::HAZE:
                    tr = r + (200.0f - r) * s * 0.5f + (128.0f - r) * s * 0.3f;
                    tg = g + (200.0f - g) * s * 0.5f + (128.0f - g) * s * 0.3f;
                    tb = b + (200.0f - b) * s * 0.5f + (128.0f - b) * s * 0.3f;
                    break;
                case Tone::CANDY:
                    tr = r + (255.0f - r) * s * 0.6f + (g - r) * s * 0.3f;
                    tg = g + (r * 0.3f - g) * s * 0.4f + (b * 0.2f - g) * s * 0.3f;
                    tb = b + (255.0f - b) * s * 0.5f + (r * 0.2f - b) * s * 0.3f;
                    break;
                case Tone::LAVA:
                    tr = r + (255.0f - r) * s * 0.8f;
                    tg = g + (60.0f - g) * s * 0.6f + (r * 0.2f - g) * s * 0.3f;
                    tb = b - b * s * 0.8f;
                    break;
                case Tone::ALIEN:
                    tr = r - r * s * 0.4f + (g * 0.5f - r) * s * 0.4f;
                    tg = g + (255.0f - g) * s * 0.5f + (g - r) * s * 0.3f;
                    tb = b + (180.0f - b) * s * 0.4f + (g * 0.3f - b) * s * 0.3f;
                    break;
                case Tone::VINTAGE:
                    tr = r + (210.0f - r) * s * 0.3f + (128.0f - r) * s * 0.2f + (g * 0.1f - r) * s * 0.2f;
                    tg = g + (180.0f - g) * s * 0.3f + (128.0f - g) * s * 0.2f;
                    tb = b + (100.0f - b) * s * 0.4f + (r * 0.05f - b) * s * 0.2f;
                    break;
                case Tone::CYBER:
                    tr = r + (r > 200 ? 255.0f - r : -r * 0.3f) * s * 0.6f + (b - r) * s * 0.3f;
                    tg = g + (b * 0.5f - g) * s * 0.5f + (255.0f - g) * s * 0.2f;
                    tb = b + (255.0f - b) * s * 0.7f + (b - r) * s * 0.3f;
                    break;
                default:
                    tr = r; tg = g; tb = b;
                    break;
            }
            
            if (tr < 0) tr = 0; if (tr > 255) tr = 255;
            if (tg < 0) tg = 0; if (tg > 255) tg = 255;
            if (tb < 0) tb = 0; if (tb > 255) tb = 255;
            
            result.push_back({(uint8_t)tr, (uint8_t)tg, (uint8_t)tb});
        }
        return result;
    }
};